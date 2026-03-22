#pragma once

#include "tile.h"


class MovableBlock : public Tile {
public:
    const sf::Texture& texture;
    MovableBlock(int x, int y, const sf::Texture& texture) : Tile(x, y), texture(texture) {
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
        posX = x;
        posY = y;
    }
    TileSignal sendSignal(const BulletHitInfo& h) const override {
        if (h.dx < 0) return TileSignal::MOVE_TILE_RIGHT; // from left
        if (h.dx > 0) return TileSignal::MOVE_TILE_LEFT;    // from right
        if (h.dy > 0) return TileSignal::MOVE_TILE_UP; // from bottom
        if (h.dy < 0) return TileSignal::MOVE_TILE_DOWN;    // from top
        return TileSignal::NONE;
    }
    int code() override {
        return 9;
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