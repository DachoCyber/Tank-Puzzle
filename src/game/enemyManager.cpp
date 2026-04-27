#include "enemyManager.h"
#include "../../include/playerInteraction.h"

EnemyManager::EnemyManager(Tank& player, Map& tileMap,
                            BulletManager& bulletManager, sf::RenderWindow& window)
    : player(player), tileMap(tileMap),
      bulletManager(bulletManager), window(window) {}

bool EnemyManager::shouldEnemyFireBullet() {
    int px = player.getGridPosition().x;
    int py = player.getGridPosition().y;

    auto tryFire = [&](int tx, int ty) -> bool {
        auto& tile = tileMap.getTileMap()[ty][tx];
        if (tile->killPlayer(tileMap.getTileMap(),
                             px * tileSize, py * tileSize))
        {
            tile->fireBullet();
            bulletManager.addBullet(tile->getBullet());
            bulletManager.coordXKillerTank = tx;
            bulletManager.coordYKillerTank = ty;
            bulletManager.setBulletFired(true);
            window.setKeyRepeatEnabled(false);
            return true;
        }
        return false;
    };

    for (int x = 0; x < MAP_DIM; x++)
        if (tryFire(x, py)) return true;

    for (int y = 0; y < MAP_DIM; y++)
        if (tryFire(px, y)) return true;

    return false;
}