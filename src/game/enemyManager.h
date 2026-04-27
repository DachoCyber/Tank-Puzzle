#pragma once
#include "../../include/map.h"
#include "../../include/player.h"
#include "BulletManager.h"
#include <SFML/Window.hpp>

class EnemyManager {
public:
    EnemyManager(Tank& player, Map& tileMap, BulletManager& bulletManager,
                 sf::RenderWindow& window);

    // vraca true ako je enemy ispalio metak
    bool shouldEnemyFireBullet();

private:
    Tank& player;
    Map& tileMap;
    BulletManager& bulletManager;
    sf::RenderWindow& window;

    const int tileSize = 32;
    static constexpr int MAP_DIM = 16;
};