#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <utility>
#include <memory>
#include <vector>

#include "bullet.h"

#include "direction.h"
#include "tileSignal.h"
#include "bulletHitInfo.h"
#include "playerTileSignal.h"

class Tile : public sf::Drawable {

protected:
    const int tileSize = 32;
    int posX, posY;

    sf::Texture tex;
    sf::Sprite sprite;

    bool walkable;
public:
 
    Tile(int x, int y) {
        posX = x;
        posY = y;
    }
    virtual ~Tile() = default;
    Tile() : walkable(false), posX(0), posY(0) {}

    virtual TileSignal sendSignal(const BulletHitInfo& hit) const {
        return TileSignal::NONE;
    }
    virtual PlayerTileSignal sendSignal() const {
        return PlayerTileSignal::NONE;
    }
    virtual int code() = 0;

    virtual void setAlpha(int alpha);
    virtual void setPosition(int x, int y) {posX = x; posY = y;} 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates) const override {
        target.draw(sprite);
    }

    virtual bool killPlayer(const std::vector<std::vector<std::unique_ptr<Tile>>>& tileMap,
                        int playerPosX, int playerPosY) = 0;

    virtual bool fireBullet() = 0;


    virtual Bullet* getBullet() {
        return nullptr;
    }
};