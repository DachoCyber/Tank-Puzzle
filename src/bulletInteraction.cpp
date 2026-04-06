#define SFML_STATIC
#include "../include/bulletInteraction.h"
#include "../include/loadSound.h"
#include "../include/bulletHitInfo.h"


BulletInteraction :: BulletInteraction(int windowSizeX, int windowSizeY, Tank& tank, Map& map) 

    : windowSizeX(windowSizeX), windowSizeY(windowSizeY), player(tank), tileMap(map) {}

void BulletInteraction :: interact() {

    if (!player.getBullet()) {
        return;
    }
    bool playBulletBubbleSound = false;
    bool playBulletHitSound = false;

    int lastBulletGridPosX = player.getBullet()->getPosition().x/tileSize;
    int lastBulletGridPosY = player.getBullet()->getPosition().y/tileSize;

    player.getBullet()->update(timePerFrame);

    if(player.getBullet()->getPosition().x >= windowSizeX || player.getBullet()->getPosition().x < 0 ||
        player.getBullet()->getPosition().y >= windowSizeY || player.getBullet()->getPosition().y < 0) {
            
        player.deleteBullet();
        return;
    }

    int bulletGridPosX = player.getBullet()->getPosition().x/tileSize;
    int bulletGridPosY = player.getBullet()->getPosition().y/tileSize;

    int lX =  lastBulletGridPosX;
    int lY =  lastBulletGridPosY;
    int x  =  bulletGridPosX;
    int y  =  bulletGridPosY;

    if (x < 0 || x > 16 || y < 0 || y > 16)
        return;

    Bullet* b = player.getBullet();
    Direction bDir = b -> getDirection();


    BulletHitInfo hit;
    hit.dir = bDir;
    hit.dx = lastBulletGridPosX - bulletGridPosX;
    hit.dy = lastBulletGridPosY - bulletGridPosY;

    int code = tileMap.getTileMapInt()[y][x];

    Tile* tile = tileMap.getTileMap()[y][x].get(); 
    TileSignal signal =  tile->sendSignal(hit);

    switch (signal) {
        
        case TileSignal::BLOCK_BULLET:
        case TileSignal::DESTROY_BULLET:    
            player.deleteBullet();
                break;
        
        case TileSignal::DESTROY_TILE:
            tileMap.destroyTile(x, y);
            player.deleteBullet();
            break;

        case TileSignal::MOVE_TILE_RIGHT:
            tileMap.moveTile(y, x + 1, y, x);
            player.deleteBullet();
            break;
        
        case TileSignal::MOVE_TILE_DOWN:
            tileMap.moveTile(y + 1, x, y, x);
            player.deleteBullet();
            break;
        
        case TileSignal::MOVE_TILE_LEFT:
            tileMap.moveTile(y, x - 1, y, x);
            player.deleteBullet();
            break;
        
        case TileSignal::MOVE_TILE_UP:
            tileMap.moveTile(y - 1, x, y, x);
            player.deleteBullet();
            break;

        case TileSignal::REFLECT_UP:
            b->setVelocity(UP);
            mirrorSound.play();
            break;

        case TileSignal::REFLECT_DOWN:
            b->setVelocity(DOWN);
            mirrorSound.play();
            break;

        case TileSignal::REFLECT_LEFT:
            b->setVelocity(LEFT);
            mirrorSound.play();
            break;

        case TileSignal::REFLECT_RIGHT:
            b->setVelocity(RIGHT);
            mirrorSound.play();
            break;
        
        case TileSignal::DESTROY_TANK_UP:
            tileMap.destroyTank(x, y, Direction::UP);
            player.deleteBullet();
            break;

        case TileSignal::DESTROY_TANK_RIGHT:
            tileMap.destroyTank(x, y, Direction::RIGHT);
            player.deleteBullet();
            break;

        case TileSignal::DESTROY_TANK_DOWN:
            tileMap.destroyTank(x, y, Direction::DOWN);
            player.deleteBullet();
            break;

        case TileSignal::DESTROY_TANK_LEFT:
            tileMap.destroyTank(x, y, Direction::LEFT);
            player.deleteBullet();
            break;

        default:
            break;
    }

    int newCode = tileMap.getTileMapInt()[y][x];

    if (newCode != code)
        mapChanged = true;
}


     