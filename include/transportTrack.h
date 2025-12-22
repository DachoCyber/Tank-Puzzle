#pragma once

#include "tile.h"
#include <iostream>

#include "bulletHitInfo.h"
    #include "tileSignal.h" 


class TransportTrack : public Tile {
    public:
    Direction dir;
    const sf::Texture& texture;
        TransportTrack(int x, int y, Direction dir,const sf::Texture& texture) : Tile(x, y), texture(texture) {
            try {
                walkable = true;
                sprite.setTexture(texture);
                sprite.setPosition(x, y);
           
                this->dir = dir;
            }
        catch(const std::string& what) {
            std::cerr << what << std::endl;
        }
    }
    PlayerTileSignal sendSignal() const override {
        switch (dir) {
            case UP: return PlayerTileSignal :: MOVE_PLAYER_UP; break;
            case DOWN: return PlayerTileSignal :: MOVE_PLAYER_DOWN; break;
            case LEFT: return PlayerTileSignal :: MOVE_PLAYER_LEFT; break;
            case RIGHT: return PlayerTileSignal :: MOVE_PLAYER_RIGHT; break;
        }
    }
    int code() override {
        switch(dir) {
            case LEFT:
                return 20;
            case RIGHT:
                return 21;
            case UP:
                return 22;
            case DOWN:
                return 23;
        }
        return -1;
    }
    void setAlpha(int alpha) override {
        sf::Color waterColor = sprite.getColor();
        waterColor.a = alpha;
        sprite.setColor(sf::Color(waterColor));
    }

    bool killPlayer(const std::vector<std::vector<std::unique_ptr<Tile>>>& tileMap,
                        int playerPosX, int playerPosY) override { return false; }
    bool fireBullet() override { return false; }
};