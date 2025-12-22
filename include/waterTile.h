#pragma once

#include "tile.h"
#include <iostream>
#include "bulletHitInfo.h"
#include "tileSignal.h" 
#include "playerTileSignal.h"  

class WaterTile : public Tile {

    private:
        const sf::Texture* texturePtr;
		const sf::Texture* secondFrameTexturePtr;
        
    public:
        WaterTile(int x, int y, const sf::Texture& WaterTileTexture, const sf::Texture& SFWaterTileTexture) : Tile(x, y) {
            walkable = false;
            texturePtr = &WaterTileTexture;
			secondFrameTexturePtr = &SFWaterTileTexture;
            sprite.setTexture(*texturePtr);
            sprite.setPosition(x, y);
            sf::Color waterColor = sprite.getColor();
            waterColor.a = 210;
            sprite.setColor(sf::Color(waterColor));
        }
        void switchFrame(int frameIndex) {
            
            
            if (frameIndex % 2 == 0) {

                sprite.setTexture(*secondFrameTexturePtr);
            } else {
				sprite.setTexture(*texturePtr);
			}
          
            
        }

    TileSignal sendSignal(const BulletHitInfo& h) const override {
        return TileSignal :: NONE;
    }

    PlayerTileSignal sendSignal() const override {
        return PlayerTileSignal :: KILL_PLAYER;
    }
    
    int code() override {
        return 8;
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