#pragma once

#include "tile.h"
#include "bulletHitInfo.h"  // MUST include argument type
#include "tileSignal.h"     // if TileSignal is separate



class TileInWater : public Tile {
public:
    const sf::Texture& texture;
    TileInWater(int x, int y, const sf::Texture& texture) : Tile(x, y), texture(texture) {
        try {
            walkable = true;
            sprite.setTexture(texture);
            sprite.setPosition(x, y);
            posX = x;
            posY = y;
        }
        catch(const std::string& what) {
            std::cerr << what << std::endl;
        }
    }
    int code() override { return 50; }

    void setAlpha(int alpha) override {
        sf::Color color = sprite.getColor();
        color.a = alpha;
        sprite.setColor(color);
    }

    bool killPlayer(const std::vector<std::vector<std::unique_ptr<Tile>>>& tileMap,
                        int playerPosX, int playerPosY) override { return false; }
    bool fireBullet() override { return false; }
};