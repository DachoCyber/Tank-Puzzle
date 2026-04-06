#pragma once

#define SFML_STATIC

#include "bulletInteraction.h"
#include "paddingGame.h"
#include "inputManager.h"
#include "gameState.h"

#include <SFML/Graphics.hpp>
#include <memory>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>




class MainGame {
public:
    MainGame(int screenSizeX, int screenSizeY, int constwindowSizeY, int constwindowSizeX, int level);
    ~MainGame() = default;
    void run();
    
    bool getWindowClosedState() const;

    bool gameEnd();
    bool gameLost();
    bool gameWon();

    std::string getInitials() const {return initials;}
    
    int getMovesCount() { return movesPlayed; }
    bool isBackClicked() { return backClicked; }


private:

    bool isTransportTrack (int code);
    void drawPadding();
    void initPadding();
    
    void undoMove();

    bool shouldEnemyFireBullet();
    bool playerKilledByEnemy();

    void handleInput();
    void render();
    void update();

    Tank player;
    Map tileMap;

    std::unique_ptr<sf::RenderWindow> window;
    
    std::vector<std::string> tankMovedOrBulletShot;
    
    bool isWindowClosed;

    int startTankX;
    int startTankY;

    int countSpacePressed;

    int currBulletGridPosX, currBulletGridPosY;
    int nextBulletGridPosX, nextBulletGridPosY;
    bool shouldBeDeleted = false;

    const int tileSize = 32;
    bool moveQueued = false;
    int windowSizeX, windowSizeY;
    int screenSizeX, screenSizeY;

    bool killPlayer = false;
    bool bulletFired = false;
	bool playerFiredBullet = false;

    bool playerMovedFromLastPosition = false;
    bool updated = false;

    bool returnFromTrack = false;

    std::vector<Bullet*> bullets;
    
    GameFrameClock clock;

    sf::Font weirdFont;

    std::string initials;

    std::vector<std::vector<std::vector<int>>> mapStates;

    std::vector<sf::Keyboard::Key> arrOfMoves;

    bool undoMoveEndGame;
    bool playerMoved = false;
    int moveCount = 0;
    std::vector<std::pair<int, int>> playerPositions;

    int playerBeginPosX, playerBeginPosY;

    int coordXKillerTank, coordYKillerTank;

    sf::Clock windowEnableRepeatMovement;
    const sf::Time windowEnableRepMovTime = sf::seconds(0.5f);
    bool isRepeatMovEnabled = false;
    int movesPlayed = 0;

    sf::Clock gameClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const sf::Time timePerFrame = sf::seconds(1.f/60.f);

    sf::Time updateFramesWaterTile = sf::seconds(0.5);
    sf::Time updateFramesEnemyTank = sf::seconds(0.33);

    sf::Clock waterFramesClock;
    sf::Clock enemyTankFramesClock;

    sf::Text backButtonText;
    float backButtonAbsPosX, backButtonAbsPosY;
    float backButtonSizeX, backButtonSizeY;
    bool backClicked;
    PaddingGame padding;
};

