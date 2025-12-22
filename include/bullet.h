#pragma once
#include <SFML/Graphics.hpp>
#include "direction.h"

class Bullet : public sf::Drawable {
public:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    Direction dir;
    bool enemysBullet;

    Bullet();

    Bullet(sf::Vector2f position, Direction dir);
    Bullet(sf::Vector2f position, Direction dir, bool enemysTank);

    void changeVelocity(Direction dir, int mirrorType);

    void setVelocity(Direction bulletDir);

    void setVelocity();
    void setTransparentColor();
    void returnOldColor();

    void update(sf::Time updateTime);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(shape);
    }

    sf::Vector2f getDimensions() const;
    sf::Vector2f getPosition() const;
    Direction getDirection() const;

    void setPosition(float x, float y);
};