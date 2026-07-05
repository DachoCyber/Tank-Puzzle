#pragma once

#include "tile.h"

#include <SFML/Graphics.hpp>
#include <iostream>

#include "bulletHitInfo.h"
#include "tileSignal.h"
#include "tileCode.h"

class DestructibleBlock : public Tile {
private:
    sf::Texture destructibleBlockTex;
    sf::Sprite destructibleBlockSprite;
    const sf::Texture& texture;
public:
    DestructibleBlock(int x, int y,const sf::Texture& texture) :Tile(x, y), texture(texture) {
        try {
            walkable = false;
            sprite.setTexture(texture);
            sprite.setPosition(x, y);
            sprite.setScale(sf::Vector2f(32.f/50.f, 32.f/52.f));
        }
        catch(const std::string& what) {
            std::cerr << what << std::endl;
        }
    }
    TileSignal sendSignal(const BulletHitInfo& h) const override {
        return TileSignal :: DESTROY_TILE;
    }
    PlayerTileSignal sendSignal() const override {
        return PlayerTileSignal::BLOCK_PLAYER;
    }
    int code() override {
        return DESTRUCTIBLE_BLOCK;
    }
    void setAlpha(int alpha) override {
        sf::Color color = sprite.getColor();
        color.a = alpha;
        sprite.setColor(color);
    }

    bool killPlayer(const std::vector<std::vector<std::unique_ptr<Tile>>>& tileMap,
                        int playerPosX, int playerPosY) override { return false; }
    bool fireBullet() override { return false; }
};

