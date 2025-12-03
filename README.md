# LaserTank Clone

**LaserTank Clone** is a reinterpretation of the classic puzzle game *LaserTank*, fully written in **C++** with **SFML 2.6.2**.  
The goal is simple: navigate your tank through challenging levels, avoid obstacles, use mirrors to redirect your laser, and reach the finish tile.  
Behind this simplicity hides a deep strategic puzzle system with undo mechanics, movable blocks, enemy tanks, and diverse terrain types.

This project was created for learning, experimentation, and fun — while carefully improving the visuals and gameplay experience of the original.

---


## Build Instructions
Windows (MinGW + SFML Static Build)

Install MinGW-w64 (UCRT) — choose 32-bit or 64-bit depending on your desired build target.

Download SFML 2.6.2 (static) that exactly matches your GCC version.

You can check version of your GCC with:

gcc --version

Clone this repository:

git clone https://github.com/DachoCyber/LaserTankVSCode.git

Extract SFML into the root of the project so that the folder structure looks like this:

LaserTankVSCode/
    SFML-2.6.2/
        SFML-2.6.2/
            include/
            lib/

These libraries are required for the online high-score system and HTTP requests.

Build the game by running:
  
  mingw32-make

  You will also have to download libcurl.dll and openal32.dll to run:
    
  mingw32-make run
    
  mingw32-make clean

## Main Menu
Below is a preview of the main menu, redesigned in a retro-pixel style:

![Main Menu](images_git/mainMenuImage.png)

---

## Example Level

Here is a sample level demonstrating movement tracks, water tiles, mirrors, bricks, and multiple tanks:

![Level Screenshot](images_git/level.png)

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
- Classes:
  - `MainGame`, `Map`, `Player`, `Bullet`, `BulletInteraction` etc  
- Clean separation of:
  - Player interaction  
  - Bullet physics  
  - Tile logic  
  - Rendering  
- Cross-platform build system:
  - Windows (MinGW / MSVC)  
  - Linux (GCC / Clang)  

---

