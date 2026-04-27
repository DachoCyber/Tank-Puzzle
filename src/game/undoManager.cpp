#include "undoManager.h"

UndoManager::UndoManager(Tank& player, Map& tileMap, int& movesPlayed)
    : player(player), tileMap(tileMap), movesPlayed(movesPlayed) {}

void UndoManager::pushAction(const std::string& action) {
    tankMovedOrBulletShot.push_back(action);
}

void UndoManager::pushMapState(const std::vector<std::vector<int>>& state) {
    mapStates.push_back(state);
}

bool UndoManager::canUndo() const {
    return !tankMovedOrBulletShot.empty() || !player.getPlayerStates().empty();
}

void UndoManager::undoMove() {
    if (movesPlayed > 0)
        movesPlayed--;

    if (tankMovedOrBulletShot.empty())
        return;

    std::string lastAction = tankMovedOrBulletShot.back();

    if (lastAction == "bullet shot") {
        if (!mapStates.empty()) {
            tileMap.undoMove(&mapStates.back());
            mapStates.pop_back();
        }
    } else if (lastAction == "tank moved") {
        if (!player.getPlayerStates().empty()) {
            player.setGridPosition(player.getPlayerStates().back().playerPos);
            player.setDir(player.getPlayerStates().back().dir);
            player.getPlayerStates().pop_back();
        }
    }

    tankMovedOrBulletShot.pop_back();
}