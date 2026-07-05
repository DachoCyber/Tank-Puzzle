#include "../../include/player.h"


void Tank::fireBullet() {

    sf::Vector2f tankPos = sprite.getPosition();
    auto tankSize = tankImg.getSize();
    float bulletStartPosX, bulletStartPosY;
    switch (dir) {
        case UP: bulletStartPosX = tankPos.x; bulletStartPosY = tankPos.y ;  break;
        case DOWN: bulletStartPosX = tankPos.x ; bulletStartPosY = tankPos.y;  break;
        case RIGHT: bulletStartPosX = tankPos.x; bulletStartPosY = tankPos.y ;  break;
        case LEFT: bulletStartPosX = tankPos.x; bulletStartPosY = tankPos.y;  break;
    }
    bullet   = new Bullet(sf::Vector2f(bulletStartPosX, bulletStartPosY), dir);

}

void Tank::deleteBullet() {
    if(bullet) {

        bullet = nullptr;
    }
}

bool Tank :: deleteOrMoveAdjBlockIfExists(Map& tileMap) {
    int x = getGridPosition().x;
    int y = getGridPosition().y;

    int newX = x, newY = y;
    switch (dir) {
        case UP    : newY -= 1;  break;
        case DOWN  : newY += 1;  break;
        case LEFT  : newX -= 1;  break;
        case RIGHT : newX += 1;  break;
    }

    if (newX < 0 || newX >= 16 || newY < 0 || newY >= 16)
        return false;


    BulletHitInfo hit;
    hit.dir = dir;
    hit.dx = x - newX;
    hit.dy = y - newY;

    int code = tileMap.getTileMapInt()[newY][newX];
    Tile* tile = tileMap.getTileMap()[newY][newX].get();
    TileSignal signal =  tile->sendSignal(hit);

    switch (signal) {
        case TileSignal::BLOCK_BULLET:
        case TileSignal::DESTROY_BULLET:
            break;

        case TileSignal::DESTROY_TILE:
            tileMap.destroyTile(x, y);
            break;

        case TileSignal::MOVE_TILE_RIGHT:
            tileMap.moveTile(y, x + 1, y, x);
            break;

        case TileSignal::MOVE_TILE_DOWN:
            tileMap.moveTile(y + 1, x, y, x);
            break;

        case TileSignal::MOVE_TILE_LEFT:
            tileMap.moveTile(y, x - 1, y, x);
            break;

        case TileSignal::MOVE_TILE_UP:
            tileMap.moveTile(y - 1, x, y, x);
            break;
        default:
            break;
    }

    return false;
}
