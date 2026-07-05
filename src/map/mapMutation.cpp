#include "../../include/map.h"
#include "../../include/tileFactory.h"
#include "../../include/tileCode.h"

void Map::destroyTank(int x, int y, int tankDir) {
    switch (tankDir) {
        case Direction::DOWN:
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 0, destroyedTankLeftTexture);
            tileMap[y][x] = DESTROYED_TANK_LEFT;
            break;

        case Direction::RIGHT:
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 3, destroyedTankUpTexture);
            tileMap[y][x] = DESTROYED_TANK_UP;
            break;

        case Direction::UP:
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 2, destroyedTankDownTexture);
            tileMap[y][x] = DESTROYED_TANK_DOWN;
            break;

        case Direction::LEFT:
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 1, destroyedTankRightTexture);
            tileMap[y][x] = DESTROYED_TANK_RIGHT;
            break;
    }
}

void Map :: destroyTile(int gridPosX, int gridPosY) {
    std::unique_ptr<Tile> tile = std::make_unique<WalkableGround>(gridPosX*tileSize, gridPosY*tileSize, walkableTexture);
    tiles[gridPosY][gridPosX] = std::move(tile);
    tileMap[gridPosY][gridPosX] = WALKABLE_GROUND;
}

void Map::setTilesEl(size_t i, size_t j, std::unique_ptr<Tile> newElement) {
    if (i < tiles.size() && j < tiles[i].size()) {
        tiles[i][j] = std::move(newElement);
    }
}

void Map::undoMove(std::vector<std::vector<int>>* lastMapState) {
    for (int y = 0; y < mapHeight; ++y) {
        auto& lastRow = (*lastMapState)[y];
        auto& tileRow = tiles[y];
        auto& mapRow = tileMap[y];

        for (int x = 0; x < mapWidth; ++x) {
            int code = lastRow[x];
            auto tile = TileFactory::constructTile(code, x, y, tileSize);

            waterTilesCoords[y][x] = code == WATER;
            movableBlockInWater[y][x] = code == TILE_IN_WATER;
            flagCoords[y][x] = code == FLAG;

            tileRow[x] = std::move(tile);
            mapRow[x] = code;
        }
    }
}
