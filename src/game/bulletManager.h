#pragma once
#include <vector>
#include <SFML/System/Time.hpp>
#include "../../include/bullet.h"
#include "../../include/player.h"

class BulletManager {
public:
    BulletManager(Tank& player, int windowSizeY, int tileSize);

    void update();
    void clearBullets();
    void addBullet(Bullet* bullet);

    bool isKillPlayer() const      { return killPlayer; }
    bool isBulletFired() const     { return bulletFired; }
    bool hasBullets() const        { return !bullets.empty(); }

    void setKillPlayer(bool v)     { killPlayer = v; }
    void setBulletFired(bool v)    { bulletFired = v; }

    std::vector<Bullet*>& getBullets() { return bullets; }

    int coordXKillerTank = 0;
    int coordYKillerTank = 0;

private:
    Tank& player;
    int windowSizeY;
    const int tileSize;

    std::vector<Bullet*> bullets;
    bool killPlayer  = false;
    bool bulletFired = false;

    static constexpr float MAP_WIDTH = 512.f;
};