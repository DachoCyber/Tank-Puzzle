#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "paddingGame.h"
#include "map.h"
#include "player.h"

class GameRenderer : public sf::Drawable {

private: 
    Tank player;
    Map tileMap;
    std::vector<Bullet*> bulletsRender;
public:



    virtual void draw (sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(tileMap);
        target.draw(player);
        for (int i = 0; i < bulletsRender.size(); i++) {
            target.draw(*bulletsRender[i]);
        }
        target.draw(padding);

    }
    std::unique_ptr<sf::RenderWindow> window;
    PaddingGame padding;
};