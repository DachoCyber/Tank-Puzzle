#pragma once
#include <SFML/Graphics.hpp>
#include "../../include/map.h"
#include "../../include/player.h"
#include "../../include/paddingGame.h"
#include "bulletManager.h"

class Renderer {
public:
    Renderer(sf::RenderWindow& window, Map& tileMap,
             Tank& player, BulletManager& bulletManager,
             PaddingGame& padding);

    void render();

private:
    sf::RenderWindow& window;
    Map& tileMap;
    Tank& player;
    BulletManager& bulletManager;
    PaddingGame& padding;
};