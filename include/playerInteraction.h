#pragma once

#include "map.h"
#include "player.h"

#include "playerTileSignal.h"
#include "tileCode.h"

inline bool validMove(int x, int y) {
    return tileMapPositionInBounds(x, y);
}

class PlayerInteraction {
private:
    int windowSizeX, windowSizeY;
    Tank& player;
    Map& tileMap;
    sf::Keyboard :: Key pressedKey;
    const int tileSize = 32;
    bool blockInput = false; // on conveyer
    bool playerMoved = false;
    bool playerTurned = false;
    bool bulletFired = false;

public:
    PlayerInteraction(int windowSizeX, int windowSizeY, Tank& player, Map& map, sf::Keyboard::Key key);
    ~PlayerInteraction() = default;
    void handlTransportableTrack(int x, int y, bool retrunFromTrack);
    void handleMovement();
    
    void setBlockInput(bool block); 
    
    void handlePlayerTileSignal();
    void movePlayer (int newX, int newY);
    void handleFire ();
    bool getPlayerMoved() const;
    bool getPlayerTurned() const;
    bool getBulletFired() const;
};