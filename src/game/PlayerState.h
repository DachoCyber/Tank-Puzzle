#pragma once
#include "../../include/player.h"
#include "bulletManager.h"

namespace state {

class PlayerState {
public:
    PlayerState(Tank& player, BulletManager& bulletManager);

    bool isDead() const;
    bool isAlive() const;
    bool hitByBullet() const;
    bool steppedOnDeadlyTile() const;

private:
    Tank& player;
    BulletManager& bulletManager;
};

}
