#include "MapState.h"

namespace state {

MapState::MapState(Tank& player, Map& tileMap)
    : player(player), tileMap(tileMap) {}

int MapState::tileUnderPlayer() const {
    sf::Vector2i pos = player.getGridPosition();
    return tileMap.getTileMapInt()[pos.y][pos.x];
}

bool MapState::reachedGoal() const {
    return tileUnderPlayer() == GOAL_TILE;
}

}
