#include "../include/playerInteraction.h"


PlayerInteraction :: PlayerInteraction(int windowSizeX, int windowSizeY, Tank& player, Map& map, sf::Keyboard::Key key) :
        windowSizeX(windowSizeX),
        windowSizeY(windowSizeY),
        player(player),
        tileMap(map),
        pressedKey(key) 
{
    
}

void PlayerInteraction :: handleMovement () {

    if (blockInput)
        return;

    if (pressedKey == sf::Keyboard::Key::Unknown || pressedKey == sf::Keyboard::Key::Space)
        return;

    int newX = player.getGridPosition().x, newY = player.getGridPosition().y;
    int x = player.getGridPosition().x, y = player.getGridPosition().y;

    sf::Vector2i pos (x, y);
    sf::Vector2i newPos (newX, newY);

    Direction dir = player.getDir();
    Direction newDir;
    
    switch (pressedKey) {
        case sf::Keyboard::Key::Up     : newDir = UP;    newY += -1; break;
        case sf::Keyboard::Key::Down   : newDir = DOWN;  newY +=  1; break;
        case sf::Keyboard::Key::Left   : newDir = LEFT;  newX += -1; break;
        case sf::Keyboard::Key::Right  : newDir = RIGHT; newX +=  1; break;
        default : break;
    }

    if (newDir != dir) {
        player.getPlayerStates().push_back({dir, pos});
        player.setDir(newDir);
        player.turnSound.play();
        playerTurned = true;
        return;
    }

    if (!validMove(newX, newY))
        return;

    int code = tileMap.getTileMapInt()[newY][newX];

    if (tileIsPlayerWalkable(code) || code == PLAYER_START)
    {

        player.setGridPosition(sf::Vector2i(newX, newY));
        player.getPlayerStates().push_back({dir, pos});
        player.moveSound.play();
        playerMoved = true;
    }
}

bool PlayerInteraction::getPlayerTurned() const {
    return playerTurned;
}

bool PlayerInteraction :: getPlayerMoved() const {
    return playerMoved;
}

void PlayerInteraction::setBlockInput(bool block) {
    blockInput = block;
}

void PlayerInteraction :: handleFire () {

    if(pressedKey == sf::Keyboard::Key::Space && !player.getBullet()) {  
        if(!player.deleteOrMoveAdjBlockIfExists(tileMap)) {
            player.fireBullet();
            bulletFired = true;
        }
        player.fireSound.play();
    }

}

bool PlayerInteraction :: getBulletFired() const {
    return bulletFired;
}

void PlayerInteraction :: movePlayer (int newX, int newY) {
    if (!validMove(newX, newY))
        return;

    int code = tileMap.getTileMapInt()[newY][newX];

    if (tileIsPlayerWalkable(code))  {

        player.setGridPosition(sf::Vector2i(newX, newY));
        blockInput = true;
    }
    else 
    {
        blockInput = false;
    }
}

void PlayerInteraction :: handlePlayerTileSignal ()
{

    int x = player.getGridPosition().x;
    int y = player.getGridPosition().y;

    Tile* tile = tileMap.getTileMap()[y][x].get();
    PlayerTileSignal signal = tile -> sendSignal();

    switch (signal) {

        case PlayerTileSignal :: MOVE_PLAYER_DOWN :
            movePlayer(x, y + 1);
            break;
        
        case PlayerTileSignal :: MOVE_PLAYER_UP :
            movePlayer(x, y - 1);
            break;
        
        case PlayerTileSignal :: MOVE_PLAYER_RIGHT :
            movePlayer(x + 1, y);
            break;

        case PlayerTileSignal :: MOVE_PLAYER_LEFT :
            movePlayer(x - 1, y);
            break;
        
        case PlayerTileSignal :: KILL_PLAYER :
            player.setShouldKillPlayer();
            break;
        
        case PlayerTileSignal :: BLOCK_PLAYER :
            break;      // nothing to be done
        
        case PlayerTileSignal :: NONE :
            break;      // just a formality

        default :
            break;
    }


}
