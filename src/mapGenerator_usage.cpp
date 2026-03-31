// ─────────────────────────────────────────────────────────────────────────────
//  Primjer integracije mapGenerator.h u LaserTankVSCode
//  Dodaj ove pozive u relevantne dijelove svog koda
// ─────────────────────────────────────────────────────────────────────────────

#include "../include/mapGenerator.h"
#include "../include/tileFactory.h"

// ─────────────────────────────────────────────────────────────────────────────
//  OPCIJA A: Generiši mapu i učitaj direktno iz grid-a u memoriji
//  (brže, ne čeka download TMX-a)
//
//  Pozovi ovo npr. kad igrač klikne "Generate AI Level" u meniju
// ─────────────────────────────────────────────────────────────────────────────
bool generateAndLoadAIMap(
    std::vector<std::vector<int>>& tileMap,          // tvoj postojeći tileMap[16][16]
    std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,  // tvoji tile objekti
    int tileSize,
    const std::string& difficulty = "medium")
{
    MapGeneratorClient gen;  // localhost:5000 po defaultu
    GeneratedMap result = gen.generateMap(difficulty);

    if (!result.success) {
        std::cerr << "AI mapa nije generisana: " << result.errorMessage << "\n";
        return false;
    }

    if (result.grid.size() != 16) {
        std::cerr << "Neispravan grid od servera\n";
        return false;
    }

    TileFactory factory;

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            int code = result.grid[y][x];
            tileMap[y][x] = code;
            tiles[y][x] = factory.constructTile(code, x, y, tileSize);
        }
    }

    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
//  OPCIJA B: Generiši i snimi TMX na disk, onda ga učitaj normalnim tokom
//  Korisno ako već imaš TMX loader i hoćeš da mapa bude sačuvana u maps/ folderu
// ─────────────────────────────────────────────────────────────────────────────
bool generateAndSaveAIMap(const std::string& difficulty = "medium") {
    MapGeneratorClient gen;
    GeneratedMap result = gen.generateMap(difficulty);

    if (!result.success) {
        std::cerr << "Greška: " << result.errorMessage << "\n";
        return false;
    }

    // Snimi lokalno u maps/ folder
    std::string localPath = "maps/" + result.filename;
    bool saved = gen.downloadMap(result.filename, localPath);

    if (saved) {
        // Sada možeš da je učitaš tinyxml2 parserom kao i svaku drugu mapu
    }
    return saved;
}

// ─────────────────────────────────────────────────────────────────────────────
//  SFML primjer – poziv u event loop-u
//  Dodaj ovo u dio koda gdje obrađuješ input u meniju ili u igri
// ─────────────────────────────────────────────────────────────────────────────

/*
// Negdje u game loop / menu loop:
if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
    // Pokreni u zasebnoj niti da SFML render ne zamrzne dok čeka API odgovor
    std::thread([&]() {
        generateAndLoadAIMap(tileMap, tiles, tileSize, "medium");
    }).detach();
}
*/

// ─────────────────────────────────────────────────────────────────────────────
//  SFML primjer – difficulty izbor (easy/medium/hard) iz menija
// ─────────────────────────────────────────────────────────────────────────────

/*
std::string selectedDifficulty = "medium";

// U event loopu:
if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Num1) selectedDifficulty = "easy";
    if (event.key.code == sf::Keyboard::Num2) selectedDifficulty = "medium";
    if (event.key.code == sf::Keyboard::Num3) selectedDifficulty = "hard";
    if (event.key.code == sf::Keyboard::G) {
        std::thread([&]() {
            generateAndLoadAIMap(tileMap, tiles, tileSize, selectedDifficulty);
        }).detach();
    }
}
*/
