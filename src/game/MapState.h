#pragma once
#include "../../include/player.h"
#include "../../include/map.h"
#include "../../include/tileCode.h"

namespace state {

class MapState {
public:
    MapState(Tank& player, Map& tileMap);

    bool reachedGoal() const;
    int tileUnderPlayer() const;

private:
    Tank& player;
    Map& tileMap;

    static constexpr int GOAL_TILE = FLAG;
};

}
