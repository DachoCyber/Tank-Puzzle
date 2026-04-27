#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <chrono>
#include "bullet.h"
#include "direction.h" 
#include "map.h"

struct PlayerState {
    Direction dir;
    sf::Vector2i playerPos;

    PlayerState(Direction dir, sf::Vector2i playerPos)
        : dir(dir), playerPos(playerPos) {}
};

class Map;


class Tank : public sf::Drawable {
public:
    Tank(int startX, int startY, int windowSizeX, int windowSizeY);
    virtual ~Tank() = default; 

    void setDir(Direction dir);
    Direction getDir() const;

    void setShouldKillPlayer() {
        shouldKillPlayer = true;
    }
    void setDontKillPlayer() {
        shouldKillPlayer = false;
    }
    sf::Sound moveSound;
    sf::Sound turnSound;
    sf::Sound fireSound;
    sf::SoundBuffer moveSoundBfr;
    sf::SoundBuffer turnSoundBfr;
    sf::SoundBuffer fireSoundBfr;
    std::vector<PlayerState>& getPlayerStates() { 
        return playerStates; 
    }
    bool getShouldKillPlayer();

    void update(sf::Time, sf::Time);
    sf::Vector2i getGridPosition() const;
    void setGridPosition(sf::Vector2i);

    bool validMove(int x, int y) const;
    
    void loadTextureFromImage();
    void updateSpritePosition();
    void deleteBullet();

    bool deleteOrMoveAdjBlockIfExists(Map& tileMap); /// Do not fire bullet
    void fireBullet();
    Bullet* getBullet() const {return  bullet;}
    bool bulletInScreen() const;
    bool getShouldFireBullet() const {return shouldFireBullet;}


    void movePlayer(int x, int y) {
        sprite.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
    } 

    int getPositionX() const {return sprite.getPosition().x;}
    int getPositionY() const {return sprite.getPosition().y;}

private:

    

    std::vector<PlayerState> playerStates;

    bool shouldKillPlayer = false;

    bool shouldFireBullet;


    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    Bullet* bullet = nullptr;

    sf::Image tankImg;
    sf::Texture texture;
    sf::Sprite sprite;

    Direction dir;
    sf::Vector2i gridPos;
    bool moveQueued = false;
    const int tileSize = 32;

    int windowSizeX, windowSizeY;
};
