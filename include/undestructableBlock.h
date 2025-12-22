#pragma once

#include "tile.h"
#include "tileSignal.h"

class UndestructableBlock : public Tile {
public:
const sf::Texture& texture;
    UndestructableBlock(int x, int y, const sf::Texture& texture) : Tile(x, y), texture(texture) {
        try {
            walkable = true;
            sprite.setTexture(texture);
            sprite.setPosition(x, y);
        }
        catch(const std::string& what) {
            std::cerr << what << std::endl;
        }
    }
    int code() override {
        return 11;
    }
    TileSignal sendSignal(const BulletHitInfo& h) const override {
        return TileSignal :: DESTROY_BULLET;
    }
    
    bool killPlayer(const std::vector<std::vector<std::unique_ptr<Tile>>>& tileMap,
                        int playerPosX, int playerPosY) override { return false; }
    bool fireBullet() override { return false; }

    PlayerTileSignal sendSignal() const override {
        return PlayerTileSignal :: BLOCK_PLAYER;
    }
    void setAlpha(int alpha) override {
        sf::Color color = sprite.getColor();
        color.a = alpha;
        sprite.setColor(color);
    }
};