#include "gameStateChecker.h"

GameStateChecker::GameStateChecker(Tank& player, Map& tileMap,
                                   BulletManager& bulletManager)
    : playerState(player, bulletManager), mapState(player, tileMap) {}

bool GameStateChecker::gameLost() const {
    return playerState.isDead();
}

bool GameStateChecker::gameWon() const {
    return mapState.reachedGoal();
}

bool GameStateChecker::gameEnd() const {
    return gameLost() || gameWon();
}
