#pragma once

#include "tile.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class WalkableGround : public Tile {
public:
    const sf::Texture& texture;
    WalkableGround(int x, int y, const sf::Texture& texture) : Tile(x, y), texture(texture) {
        try {
            walkable = true;
            sprite.setTexture(texture);
            sprite.setPosition(x, y);
            //sprite.setScale(32.f/55.f, 32.f/55.f);
        }
        catch(const std::string& what) {
            std::cerr << what << std::endl;
        }
    }
    int code() override {
        return 1;
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

