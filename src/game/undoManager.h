#pragma once
#include <vector>
#include <string>
#include "../../include/player.h"
#include "../../include/map.h"

class UndoManager {
public:
    UndoManager(Tank& player, Map& tileMap, int& movesPlayed);

    void undoMove();
    void pushAction(const std::string& action);  // "tank moved" | "bullet shot"
    void pushMapState(const std::vector<std::vector<int>>& state);
    bool canUndo() const;

    const std::vector<std::string>& getActions() const { return tankMovedOrBulletShot; }
    std::vector<std::string>& getActions() { return tankMovedOrBulletShot; }
    std::vector<std::vector<std::vector<int>>>& getMapStates() { return mapStates; }

private:
    Tank& player;
    Map& tileMap;
    int& movesPlayed;

    std::vector<std::string> tankMovedOrBulletShot;
    std::vector<std::vector<std::vector<int>>> mapStates;
};