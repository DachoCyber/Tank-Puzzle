#include "../include/player.h"
#include "../include/loadSound.h"


Tank::Tank(int gridPosX, int gridPosY, int windowSizeX, int windowSizeY) 
    :
    dir(RIGHT),
    windowSizeX(windowSizeX),
    windowSizeY(windowSizeY),
    playerStates({PlayerState{dir, sf::Vector2i(gridPosX, gridPosY)}})
{
    tankImg.loadFromFile("Images/tank.png");
    texture.loadFromImage(tankImg);
    sprite.setTexture(texture);
    
    sprite.setScale(sf::Vector2f(0.65, 0.65));  
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    setDir(dir);
    sprite.setPosition(sf::Vector2f(static_cast<float>(gridPosX * tileSize + tileSize/2), static_cast<float>(gridPosY*tileSize + tileSize/2)));
    updateSpritePosition();

    
    moveSoundBfr.loadFromFile("sounds/MOVE.MP3");
    moveSound.setBuffer(moveSoundBfr);
    turnSoundBfr.loadFromFile("sounds/TURN.MP3");
    turnSound.setBuffer(turnSoundBfr);
    fireSoundBfr.loadFromFile("sounds/FIRE.MP3");
    fireSound.setBuffer(fireSoundBfr);

    
}

bool Tank :: getShouldKillPlayer() {
    return shouldKillPlayer;
}

bool Tank::bulletInScreen() const {
    if (bullet && bullet->getPosition().x <= windowSizeX &&
        bullet->getPosition().x >= 0 &&
        bullet->getPosition().y <= windowSizeY &&
        bullet->getPosition().y >= 0) {
        
        return true;
    } else {
        return false;
    }
}


void Tank::setDir(Direction d) {
    dir = d;
    switch (dir) {
        case UP:    sprite.setRotation(0.f); break;
        case RIGHT: sprite.setRotation(90.f); break;
        case DOWN:  sprite.setRotation(180.f); break;
        case LEFT:  sprite.setRotation(270.f); break;
    }
}

Direction Tank::getDir() const {
    return dir;
}

sf::Vector2i Tank::getGridPosition() const {
    return gridPos;
}

void Tank :: setGridPosition(sf::Vector2i gridPos) {
    this -> gridPos = gridPos;
    updateSpritePosition();
}

void Tank::update(sf::Time accTime, sf::Time updateInterval) {

    sprite.setPosition(gridPos.x * tileSize + tileSize / 2, gridPos.y * tileSize + tileSize / 2);

    if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
        moveQueued = false;
    }
    if(accTime >= updateInterval && bullet) {

        bullet->update(updateInterval);
        accTime -= updateInterval;
    }
}

void Tank::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(sprite);
    if (bullet && bulletInScreen()) {

        target.draw(*bullet);
    }
}

void Tank::updateSpritePosition() {
    sprite.setPosition(sf::Vector2f(static_cast<float>(gridPos.x*tileSize + tileSize/2), static_cast<float>(gridPos.y*tileSize + tileSize/2)));
}

bool Tank :: validMove(int gridPosX, int gridPosy) const {
    return gridPosX >= 0 && gridPosX < 16 && gridPosy >= 0 && gridPosy < 16;
}
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