#pragma once
#include "../../include/player.h"
#include "../../include/map.h"
#include "bulletManager.h"
#include "PlayerState.h"
#include "MapState.h"

class GameStateChecker {
public:
    GameStateChecker(Tank& player, Map& tileMap,
                     BulletManager& bulletManager);

    bool gameLost() const;
    bool gameWon()  const;
    bool gameEnd()  const;

private:
    state::PlayerState playerState;
    state::MapState    mapState;
};
