#pragma once

#include "tile.h"
#include "tileCode.h"

class Flag : public Tile {
public:
    const sf::Texture& texture;
    Flag(int x, int y,const sf::Texture& texture) : Tile(x, y), texture(texture) {
        try {
        } 
        catch(const std::string& what) {
            std::cerr << what << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        //sprite.setScale(32.f/571.f, 32.f/574.f);
    }
    int code() override {
        return FLAG;
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