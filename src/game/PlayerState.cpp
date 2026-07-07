#include "PlayerState.h"

namespace state {

PlayerState::PlayerState(Tank& player, BulletManager& bulletManager)
    : player(player), bulletManager(bulletManager) {}

bool PlayerState::steppedOnDeadlyTile() const {
    return player.getShouldKillPlayer();
}

bool PlayerState::hitByBullet() const {
    return bulletManager.isKillPlayer();
}

bool PlayerState::isDead() const {
    return steppedOnDeadlyTile() || hitByBullet();
}

bool PlayerState::isAlive() const {
    return !isDead();
}

}
