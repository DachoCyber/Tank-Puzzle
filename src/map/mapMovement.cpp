#include "../../include/map.h"
#include "../../include/tileFactory.h"
#include "../../include/tileCode.h"

void Map::updateTransportTracks() {
    for (auto& coord : trackTileCoords) {
        int y = std::get<0>(coord);
        int x = std::get<1>(coord);
        Direction dir = std::get<2>(coord);

        int code = tiles[y][x] -> code();
        if (!tileIsTransportTrack(code) && code == WALKABLE_GROUND) {
            switch(dir) {
                case LEFT:
                    tiles[y][x] = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, dir, transportTrackLeftTex);
                    break;
                case RIGHT:
                    tiles[y][x] = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, dir, transportTrackRightTex);
                    break;
                case UP:
                    tiles[y][x] = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, dir, transportTrackUpTex);
                    break;
                case DOWN:
                    tiles[y][x] = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, dir, transportTrackDownTex);
                    break;
            }

        }
    }
}

void Map :: moveTile(int newGridPosY, int newGridPosX, int oldGridPosY, int oldGridPosX) {
    if (oldGridPosY < 0 || oldGridPosY >= 16 ||
        oldGridPosX < 0 || oldGridPosX >= 16 ||
        newGridPosY < 0 || newGridPosY >= 16 ||
        newGridPosX < 0 || newGridPosX >= 16 ||
        !tiles[oldGridPosY][oldGridPosX]) {
        return;
    }

    if (!tileIsBlockPushTarget(tileMap[newGridPosY][newGridPosX]))
    {

        return;
    }



    if (tileMap[oldGridPosY][oldGridPosX] != MOVABLE_BLOCK) {

        tileMap[newGridPosY][newGridPosX] = waterTilesCoords[newGridPosY][newGridPosX] ? WATER : tileMap[oldGridPosY][oldGridPosX];
        tileMap[oldGridPosY][oldGridPosX] = WALKABLE_GROUND;
    }
    else if (tileMap[oldGridPosY][oldGridPosX] == MOVABLE_BLOCK) {

        if (tileMap[newGridPosY][newGridPosX] == WATER) {
            tileMap[newGridPosY][newGridPosX] = TILE_IN_WATER;
            movableBlockInWater[newGridPosY][newGridPosX] = true;
        } else {
            tileMap[newGridPosY][newGridPosX] = MOVABLE_BLOCK;
        }

        if (movableBlockInWater[oldGridPosY][oldGridPosX]) {
            tileMap[oldGridPosY][oldGridPosX] = TILE_IN_WATER;
        } else {
            tileMap[oldGridPosY][oldGridPosX] = WALKABLE_GROUND;
        }


    }

    waterTilesCoords[oldGridPosY][oldGridPosX] = tileMap[oldGridPosY][oldGridPosX] == WATER;
    waterTilesCoords[newGridPosY][newGridPosX] = tileMap[newGridPosY][newGridPosX] == WATER;

    std::unique_ptr<Tile> tileNew = TileFactory :: constructTile (tileMap[newGridPosY][newGridPosX], newGridPosX, newGridPosY, 32);
    std::unique_ptr<Tile> tileOld = TileFactory :: constructTile (tileMap[oldGridPosY][oldGridPosX], oldGridPosX, oldGridPosY, 32);

    tiles[newGridPosY][newGridPosX] = std::move(tileNew);
    tiles[oldGridPosY][oldGridPosX] = std::move(tileOld);
}
