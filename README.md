# LaserTank Clone

**LaserTank Clone** is a modern reinterpretation of the classic puzzle game *LaserTank*, fully written in **C++** with **SFML 2.6.2**.  
The goal is simple: navigate your tank through challenging levels, avoid obstacles, use mirrors to redirect your laser, and reach the finish tile.  
Behind this simplicity hides a deep strategic puzzle system with undo mechanics, movable blocks, enemy tanks, and diverse terrain types.

This project was created for learning, experimentation, and fun — while carefully improving the visuals and gameplay experience of the original.

---

## Main Menu
Below is a preview of the main menu, redesigned in a retro-pixel style:

![Main Menu](images/main_menu.png)

---

## Example Level

Here is a sample level demonstrating movement tracks, water tiles, mirrors, bricks, and multiple tanks:

![Level Screenshot](images/level_preview.png)

# Features

# Gameplay
- Fully functional **tank movement system**  
- **Laser firing & bouncing** using 4 types of mirrors  
- **Movable blocks** that interact with bullets  
- **Enemy tanks** that can be destroyed  
- **Water, grass, bricks, tracks, flags**, and more  
- **Undo system (U key)** capable of restoring:
  - tank position  
  - block movement  
  - bullet state  
  - full map state  

### Level System
- Loads `.tmx` maps (Tiled Editor)  
- Level selector with preview  
- Built-in **Level Editor**

###  Graphics
- Custom **pixel-art tile set**  
- Retro UI elements  
- Smooth scaling with SFML  
- Distinct visual appearance for each block type

###  Audio
- Mirror bounce sound  
- Brick hit sound  
- Game Over sound (`game-over.mp3`)  
- Ambient and interaction effects using SFML Audio

# Engine Architecture
- Modularized classes:
  - `MainGame`, `TileMap`, `Player`, `Bullet`, `UndoSystem`, etc.  
- Clean separation of:
  - Player interaction  
  - Bullet physics  
  - Tile logic  
  - Rendering  
- Cross-platform build system:
  - Windows (MinGW / MSVC)  
  - Linux (GCC / Clang)  

---

## Build Instructions

### **Windows (MinGW + SFML Static Build)**

1. Install MinGW-w64 (UCRT, 32-bit or 64-bit depending on your build)  
2. Download **SFML 2.6.2 static** matching your compiler  
3. Clone this repository:
   ```sh
   git clone https://github.com/your-user/LaserTankClone.git

