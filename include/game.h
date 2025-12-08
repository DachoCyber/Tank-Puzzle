#pragma once

#define SFML_STATIC

#include "bulletInteraction.h"
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
    void drawPadding();
    void initPadding();
    
    void loadGoblet();
    void drawGoblet();

    void undoMove();

    void updateHUD(float dt);


    void loadGameOverFont();

    bool shouldEnemyFireBullet();
    bool playerKilledByEnemy();



    void setTrack();
    void handlTransportableTrack(int x, int y);

    void handleInput();
    void render();
    void update();

    std::vector<std::string> tankMovedOrBulletShot;
    std::unique_ptr<sf::RenderWindow> window;
    Tank player;
    Map tileMap;
    
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
    sf::Clock gameClock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    const sf::Time timePerFrame = sf::seconds(1.f/60.f);

    sf::Time updateFramesWaterTile = sf::seconds(0.5);
    sf::Time updateFramesEnemyTank = sf::seconds(0.33);

    sf::Clock waterFramesClock;
    sf::Clock enemyTankFramesClock;

    sf::Font weirdFont;
    sf::Text gameOverText;


    sf::Texture gobletTexture;
    sf::Sprite gobletSprite;

    std::string initials;

    std::vector<std::vector<std::vector<int>>> mapStates;

    std::vector<sf::Keyboard::Key> arrOfMoves;

    bool undoMoveEndGame;
    bool playerMoved = false;
    int moveCount = 0;
    std::vector<std::pair<int, int>> playerPositions;
    int coordXKillerTank, coordYKillerTank;

    sf::Clock windowEnableRepeatMovement;
    const sf::Time windowEnableRepMovTime = sf::seconds(0.5f);
    bool isRepeatMovEnabled = false;
    int movesPlayed = 0;

    sf::RectangleShape padding;
        // HUD elements
    sf::RectangleShape rightPad;
    sf::RectangleShape leftPad, topPad, bottomPad;

    sf::Text titleText;
    sf::Text movesText;
    sf::Text controlsText;
    sf::Text undoText;
    sf::Font font;

    sf::RectangleShape undoButton;

    // Icons
    sf::Texture tankIconTexture;
    sf::Sprite iconUp, iconDown, iconLeft, iconRight;

    // Blinking lights
    sf::CircleShape light1, light2;
    float blinkTimer = 0.f;

    // Mini-map
    sf::RectangleShape minimapFrame;
    sf::RenderTexture minimapTexture;
    sf::Sprite minimapSprite;

    // For border
    sf::RectangleShape borderLine;
    std::vector<sf::CircleShape> hudCorners;

    sf::RectangleShape backButton;
    float backButtonAbsPosX, backButtonAbsPosY;
    float backButtonSizeX, backButtonSizeY;
    sf::Text backButtonText;
    bool backClicked = false;

};

