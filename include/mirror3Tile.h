#pragma once

#include "tile.h"
#include "tileCode.h"
#include <iostream>


class Mirror3Tile : public Tile {

    private:
        const sf::Texture* texturePtr;
    public:
        Mirror3Tile(int x, int y, const sf::Texture& mirrorTex) : Tile(x, y) {
            walkable = false;
            texturePtr = &mirrorTex;
            sprite.setTexture(*texturePtr);
            sprite.setPosition(x, y);
        }

    TileSignal sendSignal(const BulletHitInfo& h) const override {
        if (h.dx > 0) return TileSignal::MOVE_TILE_LEFT; // from left
        if (h.dx < 0) return TileSignal::REFLECT_UP;    // from right
        if (h.dy > 0) return TileSignal::MOVE_TILE_UP; // from top
        if (h.dy < 0) return TileSignal::REFLECT_LEFT;    // from bottom
        return TileSignal::NONE;
    }
    PlayerTileSignal sendSignal() const override {
        return PlayerTileSignal::BLOCK_PLAYER;
    }
    int code() override {
        return MIRROR_3;
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