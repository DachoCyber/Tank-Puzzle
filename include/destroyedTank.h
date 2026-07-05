#pragma once

#include "tile.h"
#include "bullet.h"

#include "bulletHitInfo.h"
#include "tileSignal.h"
#include "tileCode.h"

#include <memory>

class DestroyedTank: public Tile {
public:
    const sf::Texture& texture;
    int dir;
    DestroyedTank(int x, int y, int dir, const sf::Texture& texture) : Tile(x, y), texture(texture) {
        try {
            this-> dir = dir;
            walkable = false;
            sprite.setTexture(texture);
            sprite.setPosition(x, y);
        }
        catch(const std::string& what) {
            std::cerr << what << std::endl;
        }
    }
    
    TileSignal sendSignal(const BulletHitInfo& h) const override {
        if (h.dx < 0) return TileSignal::MOVE_TILE_RIGHT;
        if (h.dx > 0) return TileSignal::MOVE_TILE_LEFT;
        if (h.dy < 0) return TileSignal::MOVE_TILE_DOWN;
        return TileSignal::MOVE_TILE_UP;
    }
    PlayerTileSignal sendSignal() const override {
        return PlayerTileSignal::BLOCK_PLAYER;
    }


    int code() override {
        return DESTROYED_TANK_LEFT + dir;
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