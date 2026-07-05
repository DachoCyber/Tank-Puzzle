#include "../../include/map.h"
#include "../../include/tileFactory.h"
#include "../../include/tileCode.h"

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return false;
    }
    int tile = tileMap[y][x];
    return (tile == WALKABLE_GROUND || tile == MIRROR_2 || tile == MIRROR_3);
}

sf::Vector2f Map::getTilePosition(int x, int y) const {
    return sf::Vector2f(x * tileSize, y * tileSize);
}

Tile* Map :: getTileFromUniquePtr(std::unique_ptr<Tile> tile) const {
    return tile.get();
}

std::vector<std::vector<int>> Map::getMapState() const {
    std::vector<std::vector<int>> mapState(mapHeight);

    for(int i = 0; i < mapHeight; i++) {
        mapState[i].reserve(mapWidth);
        for(int j = 0; j < mapWidth; j++) {
            mapState[i].push_back(tiles[i][j] -> code());
        }
    }
    return mapState;
}
