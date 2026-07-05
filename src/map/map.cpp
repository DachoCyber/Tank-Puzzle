#include "../../include/map.h"
#include "../../include/extractMatrix.h"
#include "../../include/tileFactory.h"
#include "../../include/tileCode.h"

Map::Map(int level) {
    readMapFromXML(tileMap, level);

    int mapWidth = 16;
    int mapHeight = 16;

    tiles.resize(mapHeight);
    for (int y = 0; y < mapHeight; ++y) {
        tiles[y].resize(mapWidth);
    }
    buildMap();
}


void Map::buildMap() {

    waterTilesCoords.resize(mapHeight, std::vector<bool>(mapWidth, false));
    movableBlockInWater.resize(mapHeight, std::vector<bool>(mapWidth, false));
    flagCoords.resize(mapHeight, std::vector<bool>(mapWidth, false));

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {

            if(tileMap[y][x] == PLAYER_START) {
                playerPosX = x;
                playerPosY = y;
            }
            if(tileMap[y][x] == WATER) {
                waterTilesCoords[y][x] = true;
            }
            if(tileMap[y][x] == FLAG) {
                flagCoords[y][x] = true;
            }
            if(tileMap[y][x] == TRANSPORT_TRACK_LEFT) {
                trackTileCoords.push_back(std::make_tuple(y, x, LEFT));
            } else if(tileMap[y][x] == TRANSPORT_TRACK_RIGHT) {
                trackTileCoords.push_back(std::make_tuple(y, x, RIGHT));
            } else if(tileMap[y][x] == TRANSPORT_TRACK_UP) {
                trackTileCoords.push_back(std::make_tuple(y, x, UP));
            } else if(tileMap[y][x] == TRANSPORT_TRACK_DOWN) {
                trackTileCoords.push_back(std::make_tuple(y, x, DOWN));
            }
            tiles[y][x] = TileFactory :: constructTile(tileMap[y][x], x, y, tileSize);
        }
    }
}


void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    for (int y = 0; y < tiles.size(); y++) {
        for (int x = 0; x < tiles[y].size(); x++) {
            if (!tiles[y][x]) continue;

            target.draw(*tiles[y][x]);
        }
    }

}
