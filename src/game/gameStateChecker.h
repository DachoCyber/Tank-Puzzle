#pragma once
#include "../../include/player.h"
#include "../../include/map.h"
#include "bulletManager.h"

class GameStateChecker {
public:
    GameStateChecker(Tank& player, Map& tileMap,
                     BulletManager& bulletManager);

    bool gameLost() const;
    bool gameWon()  const;
    bool gameEnd()  const;

private:
    Tank& player;
    Map& tileMap;
    BulletManager& bulletManager;

    static constexpr int GOAL_TILE = 10;
};