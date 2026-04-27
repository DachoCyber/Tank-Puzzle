#pragma once
#define SFML_STATIC

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <string>

#include "../../include/player.h"
#include "../../include/map.h"
#include "../../include/paddingGame.h"

#include "bulletManager.h"
#include "enemyManager.h"
#include "inputHandler.h"
#include "undoManager.h"
#include "gameStateChecker.h"
#include "renderer.h"

#include "gameLoop.h"

class MainGame {
public:
    MainGame(int screenSizeX, int screenSizeY,
             int constwindowSizeX, int constwindowSizeY, int level);
    ~MainGame() = default;

    void run();

    bool getWindowClosedState() const { return isWindowClosed; }
    bool gameWon()  const { return stateChecker.gameWon();  }
    bool gameLost() const { return stateChecker.gameLost(); }
    bool gameEnd()  const { return stateChecker.gameEnd();  }

    std::string getInitials()  const { return initials; }
    int  getMovesCount()       const { return movesPlayed; }
    bool isBackClicked()       const { return backClicked; }

private:
    // window
    std::unique_ptr<sf::RenderWindow> window;
    int windowSizeX, windowSizeY;
    int screenSizeX, screenSizeY;

    // core objekti
    Map      tileMap;
    Tank     player;
    PaddingGame padding;

    // state
    bool        isWindowClosed = false;
    bool        backClicked    = false;
    std::string initials;
    int         movesPlayed    = 0;
    bool        returnFromTrack = false;
    bool        isRepeatMovEnabled = false;

    // zvuci
    sf::SoundBuffer wonBuffer, gameOverBuffer;
    sf::Sound       levelWonSound, gameOverSound;

    // subsistemi
    BulletManager    bulletManager;
    EnemyManager     enemyManager;
    UndoManager      undoManager;
    InputHandler     inputHandler;
    GameStateChecker stateChecker;
    Renderer         renderer;
    GameLoop         gameLoop;
};