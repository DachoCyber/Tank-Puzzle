import os
import json
from dataclasses import dataclass
from typing import List
from mistralai import Mistral
import os



STATE_FILE = "tile_id_state.json"
OUT_DIR = "generated_tiles"
START_ID = 51


@dataclass
class TileSpec:
    system: str
    effects: List[str]
    tile_name: str


def load_next_id() -> int:
    if not os.path.exists(STATE_FILE):
        return START_ID
    with open(STATE_FILE, "r", encoding="utf-8") as f:
        data = json.load(f)
    return int(data.get("next_id", START_ID))


def save_next_id(next_id: int) -> None:
    with open(STATE_FILE, "w", encoding="utf-8") as f:
        json.dump({"next_id": next_id}, f, indent=2)


def parse_user_line(line: str) -> TileSpec:
    parts = [p.strip() for p in line.split(";") if p.strip()]
    if len(parts) < 2:
        raise ValueError(
            "Input must be like: BulletInteraction; reflect up; move tile down; tile"
        )

    system = parts[0]
    tile_name = parts[-1]
    effects = parts[1:-1]

    return TileSpec(system=system, effects=effects, tile_name=tile_name)


def build_prompt(tile_id: int, spec: TileSpec) -> str:
    effects_text = ", ".join(spec.effects) if spec.effects else "no special effects"

    return f"""
You are a C++ game engine assistant.

Generate a SINGLE tile implementation for a LaserTank-like puzzle game.

Tile ID: {tile_id}
System: {spec.system}
Tile name: {spec.tile_name}
Behavior keywords: {effects_text}

Output STRICTLY in this format:

--- DESCRIPTION ---
(short explanation)

--- C++ HEADER ---
(valid .h code)

--- C++ SOURCE ---
(valid .cpp code)

Rules:
- Use modern C++17
- Tile must expose sendSignal(const BulletHitInfo&)
- Use enums Direction and TileSignal
- No extra commentary
- No markdown
""".strip()


def main():
    api_key = os.getenv("MISTRAL_API_KEY")
    if not api_key:
        raise RuntimeError("Set MISTRAL_API_KEY environment variable.")

    os.makedirs(OUT_DIR, exist_ok=True)

    print("Enter tile description like:")
    print("  BulletInteraction; reflect up; move tile down; tile")
    line = input("> ").strip()

    spec = parse_user_line(line)
    tile_id = load_next_id()
    prompt = build_prompt(tile_id, spec)

    client = Mistral(api_key=os.environ["MISTRAL_API_KEY"])

    response = client.chat.complete(
        model="mistral-small-latest",
        messages=[
            {"role": "user", "content": prompt}
        ]
    )


    text = response.choices[0].message.content


    out_path = os.path.join(OUT_DIR, f"tile_{tile_id}.txt")
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(text)

    save_next_id(tile_id + 1)

    print("\nDONE")
    print(f"Tile ID: {tile_id}")
    print(f"Saved:  {out_path}")


if __name__ == "__main__":
    main()
