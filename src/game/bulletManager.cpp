#include "bulletManager.h"

BulletManager::BulletManager(Tank& player, int windowSizeY, int tileSize)
    : player(player), windowSizeY(windowSizeY), tileSize(tileSize) {}

void BulletManager::addBullet(Bullet* bullet) {
    bullets.push_back(bullet);
}

void BulletManager::clearBullets() {
    bullets.clear();
}

void BulletManager::update() {
    if (bullets.empty()) {
        bulletFired = false;
        return;
    }

    const sf::Time updateInterval = sf::seconds(1.f / 60.f);

    for (auto* b : bullets)
        b->update(updateInterval);

    sf::Vector2i playerGrid = player.getGridPosition();

    // kill detekcija + brisanje out-of-bounds
    for (int i = static_cast<int>(bullets.size()) - 1; i >= 0; i--) {
        sf::Vector2f pos = bullets[i]->getPosition();

        if (static_cast<int>(pos.x) / tileSize == playerGrid.x &&
            static_cast<int>(pos.y) / tileSize == playerGrid.y)
        {
            killPlayer = true;
        }

        if (pos.x < 0 || pos.x > MAP_WIDTH ||
            pos.y < 0 || pos.y > static_cast<float>(windowSizeY))
        {
            bullets.erase(bullets.begin() + i);
        }
    }
}