#include "../include/map.h"
#include "../include/extractMatrix.h"


#include "../include/tile.h"
#include "../include/destructibleBlock.h"
#include "../include/mirror1Tile.h"
#include "../include/mirror2Tile.h"
#include "../include/mirror3Tile.h"
#include "../include/mirror4Tile.h"
#include "../include/waterTile.h"
#include "../include/movableBlock.h"
#include "../include/undestructableBlock.h"
#include "../include/tank1.h"
#include "../include/destroyedTank.h"
#include "../include/transportTrack.h"
#include "../include/tileInWater.h"

#include "../include/flag.h"

#include "../include/tileFactory.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

Map::Map(int level) {
    readMapFromXML(tileMap, level);

    int mapWidth = 16;
    int mapHeight = 16;

    tiles.resize(mapHeight);
    for (int y = 0; y < mapHeight; ++y) {
        tiles[y].resize(mapWidth);
    }

    loadTextures();
    buildMap();
}

void Map::loadTextures() {
    if (!walkableTexture.loadFromFile("Images/walkableGround.png")) {
        throw std::runtime_error("Failed to load walkable texture");
    }
    if (!destructibleTexture.loadFromFile("Images/destructibleBlock.png")) {
        throw std::runtime_error("Failed to load destructible texture");
    }
    if(!mirror1Texture.loadFromFile("Images/Mirror1WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 1 texture");
    }
    if(!mirror2Texture.loadFromFile("Images/Mirror2WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 2 texture");
    }
    if(!mirror3Texture.loadFromFile("Images/Mirror3WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 3 texture");
    }
    if(!mirror4Texture.loadFromFile("Images/Mirror4WithBackground.png")) {
        throw std::runtime_error("Failed to load mirror 4 texture");
    }
    if(!waterTileTexture.loadFromFile("Images/waterTile.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    if(!waterSecondFrameTexture.loadFromFile("Images/waterTileSecondFrame.png")) {
        throw std::runtime_error("Failed to load water tile second frame texture");
	}
    if(!movableBlockTexture.loadFromFile("Images/MovableBlock.png")) {
        throw std::runtime_error("Failed to load water tile texture");
    }
    if(!flagTexture.loadFromFile("Images/flagWithBackground.png")) {
        throw std::runtime_error("Failed to load flag texture");
    }
    if(!undestructableBlockTex.loadFromFile("Images/undestructibleBlock.png")) {
        throw std::runtime_error("Failed to load undestructible block texture");
    }
    if(!EnemyTank1LeftTexture.loadFromFile("Images/tank1.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    if(!EnemyTank1RightTexture.loadFromFile("Images/tank2.png")) {
        throw std::runtime_error("Failed to load enemy tank left texture");
    }
    if(!EnemyTank1UpTexture.loadFromFile("Images/tank3.png")) {
        throw std::runtime_error("Failed to load enemy tank up texture");
    }
    if(!EnemyTank1DownTexture.loadFromFile("Images/tank4.png")) {
        throw std::runtime_error("Failed to load enemy tank down texture");
    }
    if(!destroyedTankLeftTexture.loadFromFile("Images/destroyedTank1.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!destroyedTankUpTexture.loadFromFile("Images/destroyedTank2.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!destroyedTankDownTexture.loadFromFile("Images/destroyedTank3.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!destroyedTankRightTexture.loadFromFile("Images/destroyedTank4.png")) {
        throw std::runtime_error("Failed to load destoryed tank left texutre");
    }
    if(!transportTrackDownTex.loadFromFile("Images/transportTrack4.png")) {
        throw std::runtime_error("Failed to load transport track texture");
    }
    if(!transportTrackUpTex.loadFromFile("Images/transportTrack3.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    if(!transportTrackRightTex.loadFromFile("Images/transportTrack2.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    if(!transportTrackLeftTex.loadFromFile("Images/transportTrack.png")) {
        throw std::runtime_error("Failed to load transport track 3 texture");
    }
    if(!tileInWaterTex.loadFromFile("Images/tileInWater.png")) {
        throw std::runtime_error("Failed to load tile in water texture");
    }
    if(!thirdFrameEnemyTank1Texture.loadFromFile("Images/tank1SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank1Texture.loadFromFile("Images/tank1ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    if(!thirdFrameEnemyTank2Texture.loadFromFile("Images/tank2SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank2Texture.loadFromFile("Images/tank2ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    if(!thirdFrameEnemyTank3Texture.loadFromFile("Images/tank3SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank3Texture.loadFromFile("Images/tank3ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }
    if(!thirdFrameEnemyTank4Texture.loadFromFile("Images/tank4SecondFrame.png")) {
        throw std::runtime_error("Failed to load tank1 second frame texture");
    }
    if(!secondFrameEnemyTank4Texture.loadFromFile("Images/tank4ThirdFrame.png")) {
        throw std::runtime_error("Failed to load tank1 third frame texture");
    }

}


void Map::buildMap() {

    waterTilesCoords.resize(mapHeight, std::vector<bool>(mapWidth, false));
    movableBlockInWater.resize(mapHeight, std::vector<bool>(mapWidth, false));

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            
            if(tileMap[y][x] == 0) {
                playerPosX = x;
                playerPosY = y;
            }
            if(tileMap[y][x] == 8) {
                waterTilesCoords[y][x] = true;
            }
            if(tileMap[y][x] == 20) {
                trackTileCoords.push_back(std::make_tuple(y, x, LEFT));
            } else if(tileMap[y][x] == 21) {
                trackTileCoords.push_back(std::make_tuple(y, x, RIGHT));
            } else if(tileMap[y][x] == 22) {
                trackTileCoords.push_back(std::make_tuple(y, x, UP));
            } else if(tileMap[y][x] == 23) {
                trackTileCoords.push_back(std::make_tuple(y, x, DOWN));
            }
            tiles[y][x] = TileFactory :: constructTile(tileMap[y][x], x, y, tileSize);
        
        }

    }
}


void Map::destroyTank(int x, int y, int tankDir) {
    switch (tankDir) {
        case Direction::DOWN:   // 3
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 0, destroyedTankLeftTexture);
            tileMap[y][x] = 16;
            break;

        case Direction::RIGHT:  // 2
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 3, destroyedTankUpTexture);
            tileMap[y][x] = 17;
            break;

        case Direction::UP:     // 1
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 2, destroyedTankDownTexture);
            tileMap[y][x] = 18;
            break;

        case Direction::LEFT:   // 4
            tiles[y][x] = std::make_unique<DestroyedTank>(x*tileSize, y*tileSize, 1, destroyedTankRightTexture);
            tileMap[y][x] = 19;
            break;
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

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) {
        return false;
    }
    int tile = tileMap[y][x];
    return (tile == 1 || tile == 2 || tile == 5 || tile == 6);
}

sf::Vector2f Map::getTilePosition(int x, int y) const {
    return sf::Vector2f(x * tileSize, y * tileSize);
}

void Map::updateTransportTracks() {
    for (auto& coord : trackTileCoords) {
        int y = std::get<0>(coord);
        int x = std::get<1>(coord);
        Direction dir = std::get<2>(coord);
        
        if (!tiles[y][x]->isTransportTrack() && tiles[y][x]->isWalkableGround()) {
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
            if(tileMap[y][x] == 12 || tileMap[y][x] == 13 || tileMap[y][x] == 14 || tileMap[y][x] == 15) {
               
                EnemyTank1* enemyTank1 = dynamic_cast<EnemyTank1*>(tiles[y][x].get());
                enemyTank1 -> switchFrame(i);
            }
        }
    }
}

void Map :: updateWaterTiles() {
}

void Map :: moveTile(int newGridPosY, int newGridPosX, int oldGridPosY, int oldGridPosX) {
    // 1. Check bounds and validity
    if (oldGridPosY < 0 || oldGridPosY >= 16 ||
        oldGridPosX < 0 || oldGridPosX >= 16 ||
        newGridPosY < 0 || newGridPosY >= 16 ||
        newGridPosX < 0 || newGridPosX >= 16 ||
        !tiles[oldGridPosY][oldGridPosX]) {
        return;
    }

    if (! (tileMap[newGridPosY][newGridPosX] == 8  ||
            tileMap[newGridPosY][newGridPosX] == 1  ||
            tileMap[newGridPosY][newGridPosX] == 50 ||
            tileMap[newGridPosY][newGridPosX] == 20 ||
            tileMap[newGridPosY][newGridPosX] == 21 ||
            tileMap[newGridPosY][newGridPosX] == 22 ||
            tileMap[newGridPosY][newGridPosX] == 23 ))
    {

        return;
    }
        
    if (tileMap[oldGridPosY][oldGridPosX] != 9) {

        tileMap[newGridPosY][newGridPosX] = waterTilesCoords[newGridPosY][newGridPosX] ? 8 : tileMap[oldGridPosY][oldGridPosX];
        tileMap[oldGridPosY][oldGridPosX] = 1;
    }
    else {

        if (tileMap[newGridPosY][newGridPosX] == 8) {
            tileMap[newGridPosY][newGridPosX] = 50;
            movableBlockInWater[newGridPosY][newGridPosX] = true;
        } else {
            tileMap[newGridPosY][newGridPosX] = 9;
        }

        if (movableBlockInWater[oldGridPosY][oldGridPosX]) {
            tileMap[oldGridPosY][oldGridPosX] = 50;
        } else {
            tileMap[oldGridPosY][oldGridPosX] = 1;
        }
    }

    waterTilesCoords[oldGridPosY][oldGridPosX] = tileMap[oldGridPosY][oldGridPosX] == 8;
    waterTilesCoords[newGridPosY][newGridPosX] = tileMap[newGridPosY][newGridPosX] == 8;

    std::unique_ptr<Tile> tileNew = TileFactory :: constructTile (tileMap[newGridPosY][newGridPosX], newGridPosX, newGridPosY, 32);
    std::unique_ptr<Tile> tileOld = TileFactory :: constructTile (tileMap[oldGridPosY][oldGridPosX], oldGridPosX, oldGridPosY, 32);
    
    tiles[newGridPosY][newGridPosX] = std::move(tileNew);
    tiles[oldGridPosY][oldGridPosX] = std::move(tileOld);
}


Tile* Map :: getTileFromUniquePtr(std::unique_ptr<Tile> tile) const {
    return tile.get();
}

std::vector<std::vector<int>> Map::getMapState() const {
    std::vector<std::vector<int>> mapState(mapHeight); // Initialize outer vector with mapHeight empty vectors
    
    for(int i = 0; i < mapHeight; i++) {
        mapState[i].reserve(mapWidth); // Optional but recommended for performance
        for(int j = 0; j < mapWidth; j++) {
            mapState[i].push_back(tiles[i][j] -> code());
        }
    }
    return mapState;
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

            waterTilesCoords[y][x] = code == 8;
            movableBlockInWater[y][x] = code == 50;

            tileRow[x] = std::move(tile);
            mapRow[x] = code;
        }
    }
}

