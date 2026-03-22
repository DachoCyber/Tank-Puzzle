#pragma once

#include <iostream>
#include "bullet.h"
#include "player.h"
#include "gameFrameClock.h"

class GameState {
public:

    GameState (int screenSizeX, int screenSizeY, int constwindowSizeX, int constwindowSizeY, int level) :
        windowSizeX (constwindowSizeX),
        windowSizeY (constwindowSizeY),
        bullets (0, nullptr)
    {

    }

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
    int coordXKillerTank, coordYKillerTank;

    sf::Clock windowEnableRepeatMovement;
    const sf::Time windowEnableRepMovTime = sf::seconds(0.5f);
    bool isRepeatMovEnabled = false;
    int movesPlayed = 0;

    sf::Text backButtonText;
    float backButtonAbsPosX, backButtonAbsPosY;
    float backButtonSizeX, backButtonSizeY;
    bool backClicked;
};