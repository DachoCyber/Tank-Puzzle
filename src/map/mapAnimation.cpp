#include "../../include/map.h"
#include "../../include/tileFactory.h"
#include "../../include/tileCode.h"

void Map::switchFramesWaterTiles(int i) {

    for(int y = 0; y < 16; y++) {
        for(int x = 0; x < 16; x++) {

            if (!waterTilesCoords[y][x])
                continue;

            WaterTile* waterTile = dynamic_cast<WaterTile*>(tiles[y][x].get());
            if(waterTile) {
                waterTile -> switchFrame(i);
            }
		}
    }
}

void Map::switchFramesEnemyTank(int i) {
    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            if(tileIsEnemyTank(tileMap[y][x])) {

                EnemyTank1* enemyTank1 = dynamic_cast<EnemyTank1*>(tiles[y][x].get());
                enemyTank1 -> switchFrame(i);
            }
        }
    }
}

void Map :: updateWaterTiles() {
}
