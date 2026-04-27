#include "gameStateChecker.h"

GameStateChecker::GameStateChecker(Tank& player, Map& tileMap,
                                   BulletManager& bulletManager)
    : player(player), tileMap(tileMap), bulletManager(bulletManager) {}

bool GameStateChecker::gameLost() const {
    return player.getShouldKillPlayer() || bulletManager.isKillPlayer();
}

bool GameStateChecker::gameWon() const {
    auto pos = player.getGridPosition();
    return tileMap.getTileMapInt()[pos.y][pos.x] == GOAL_TILE;
}

bool GameStateChecker::gameEnd() const {
    return gameLost() || gameWon();
}