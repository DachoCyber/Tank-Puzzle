#include "../include/playerInteraction.h"


PlayerInteraction :: PlayerInteraction(int windowSizeX, int windowSizeY, Tank& player, Map& map, sf::Keyboard::Key key) :
        windowSizeX(windowSizeX),
        windowSizeY(windowSizeY),
        player(player),
        tileMap(map),
        pressedKey(key) 
{
    
}

bool validMove(int x, int y) {
    return x >= 0 && x < 16  && 
           y >= 0 && y < 16;
}

void PlayerInteraction :: handleMovement () {

    if (pressedKey == sf::Keyboard::Key::Unknown || pressedKey == sf::Keyboard::Key::Space)
        return;

    int newX = player.getGridPosition().x, newY = player.getGridPosition().y;
    int x = player.getGridPosition().x, y = player.getGridPosition().y;

    sf::Vector2i pos (x, y);
    sf::Vector2i newPos (newX, newY);

    Direction dir = player.getDir();
    Direction newDir;
    
    switch (pressedKey) {
        case sf::Keyboard::Key::Up    : newDir = UP;    newY += -1; break;
        case sf::Keyboard::Key::Down  : newDir = DOWN;  newY +=  1; break;
        case sf::Keyboard::Key::Left  : newDir = LEFT;  newX += -1; break;
        case sf::Keyboard::Key::Right : newDir = RIGHT; newX +=  1; break;
        default : break;
    }

    if (newDir != dir) {
        player.setDir(newDir);
        player.turnSound.play();
        player.getPlayerStates().push_back({dir, pos});
        
        return;
    }

    if (!validMove(newX, newY))
        return;

    int code = tileMap.getTileMapInt()[newY][newX];

    // if next tile code is not walkable ground, water, tile in water etc.
    // we can not move, otherwise move player
    if (code == 1  || code == 8  || code == 50 || code == 20 ||
        code == 21 || code == 22 || code == 23 || code == 10 || code == 0)
    {

        player.setGridPosition(sf::Vector2i(newX, newY));
        player.getPlayerStates().push_back({dir, newPos});
        player.moveSound.play();
    }
}

void PlayerInteraction :: handleFire () {

    if(pressedKey == sf::Keyboard::Key::Space && !player.getBullet()) {  
        if(!player.deleteAdjBlockIfExists(tileMap)) {
            player.fireBullet();
        }

        player.fireSound.play();
    }

}

void PlayerInteraction :: movePlayer (int newX, int newY) {
    int code = tileMap.getTileMapInt()[newY][newX];

    if (!validMove(newX, newY))
        return;

    /*
        Player can move on the tile if it is:
        1) Walkable ground (1)
        2) Water tile      (8)
        3) TileInWater     (50)
        4) Transport track (UP, LEFT, RIGHT, DOWN)
        5) If it is a flag (10)
    */
    if (code == 1  || code == 8  || code == 50 || code == 20 ||
        code == 21 || code == 22 || code == 23 || code == 10)  {

        player.setGridPosition(sf::Vector2i(newX, newY));
    }

}

void PlayerInteraction :: handlePlayerTileSignal ()
{

    int x = player.getGridPosition().x;
    int y = player.getGridPosition().y;

    Tile* tile = tileMap.getTileMap()[y][x].get();
    PlayerTileSignal signal = tile -> sendSignal();

    switch (signal) {

        // transport track
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