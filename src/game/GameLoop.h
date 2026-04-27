#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../../include/player.h"
#include "../../include/map.h"
#include "../../include/paddingGame.h"

#include "bulletManager.h"
#include "enemyManager.h"
#include "inputHandler.h"
#include "undoManager.h"
#include "gameStateChecker.h"
#include "renderer.h"

class GameLoop {
public:
    GameLoop(sf::RenderWindow& window,
             Tank& player, Map& tileMap, PaddingGame& padding,
             BulletManager& bulletManager, EnemyManager& enemyManager,
             InputHandler& inputHandler, UndoManager& undoManager,
             GameStateChecker& stateChecker, Renderer& renderer,
             int windowSizeX, int windowSizeY,
             bool& backClicked, bool& isWindowClosed,
             std::string& initials);

    void run();

private:
    void processNormalFrame();
    void processEndGameFrame();
    void handleWindowEvents();
    void updateAnimations();

    sf::RenderWindow& window;
    Tank& player;
    Map& tileMap;
    PaddingGame& padding;
    BulletManager& bulletManager;
    EnemyManager& enemyManager;
    InputHandler& inputHandler;
    UndoManager& undoManager;
    GameStateChecker& stateChecker;
    Renderer& renderer;

    int windowSizeX, windowSizeY;
    bool& backClicked;
    bool& isWindowClosed;
    std::string& initials;

    bool waterToggle    = false;
    int  framesTank     = 0;
    bool undoMoveEndGame = false;
    bool isRepeatMovEnabled = false;

    sf::Clock windowEnableRepeatMovement;
    const sf::Time windowEnableRepMovTime = sf::seconds(0.5f);

    sf::Time updateFramesWaterTile  = sf::seconds(0.5f);
    sf::Time updateFramesEnemyTank  = sf::seconds(0.33f);
    sf::Clock waterFramesClock;
    sf::Clock enemyTankFramesClock;
};