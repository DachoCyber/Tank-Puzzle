# Tank-Puzzle

[![CI](https://github.com/DachoCyber/Tank-Puzzle/actions/workflows/ci.yml/badge.svg)](https://github.com/DachoCyber/Tank-Puzzle/actions/workflows/ci.yml)

Tank-Puzzle is a puzzle game where you steer a tank that fires a laser through mirrors, pushes blocks, and destroys obstacles to reach the goal tile.

## Screenshot

![Example level](images_git/level.png)

## How to play

- Arrow keys move and turn the tank. Pressing a direction turns the tank to face that way, and pressing the same direction again moves it one tile.
- Space fires the laser. If a block sits right in front of the tank, Space pushes or destroys it instead.
- U undoes the last move and restores the previous tank position, block layout, and bullet state.

Reach the flag tile to finish the level.

## How to build

The build system downloads SFML 2.6.2 automatically on the first build. On Linux and macOS you provide libcurl through the system package manager.

### Windows (MinGW)

This is a 32-bit build, so use a 32-bit MinGW-w64 toolchain (`gcc -dumpmachine` should report an `i686` target).

```
git clone https://github.com/DachoCyber/Tank-Puzzle.git
cd Tank-Puzzle
mingw32-make
mingw32-make run
```

### Linux

```
sudo apt install libsfml-dev libcurl4-openssl-dev
make
./LaserTank
```

### macOS

```
brew install sfml curl
make
./LaserTank
```

## Features

- 42 levels
- 4 types of mirrors
- Movable blocks
- Water tiles
- Transport tracks
- Enemy tanks
- Undo system
- Online score board
- Level editor

## Running tests

The unit tests are built and run with a single target:

```
make tests
```

This compiles the doctest suite in `tests/` and runs it, covering the tile factory and the tile and movement logic.
