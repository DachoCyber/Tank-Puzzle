#pragma once
#include "../include/bullet.h"
#include <iostream>


Bullet :: Bullet() {
    shape.setSize(sf::Vector2f(0.f, 0.f));
    velocity = sf::Vector2f(0.f, 0.f);
    dir = UP;
}

Bullet :: Bullet(sf::Vector2f position, Direction dir) : dir(dir), enemysBullet(false) {
    switch(dir) {
        case RIGHT : case LEFT :
            shape.setSize(sf::Vector2f(20.f, 3.f));
            break;
        case UP : case DOWN:
            shape.setSize(sf::Vector2f(3.f, 20.f));
    }
    shape.setFillColor(sf::Color(159, 43, 104));
    shape.setOutlineThickness(1.f);
    shape.setPosition(position);
    setVelocity();
}

Bullet ::Bullet(sf::Vector2f position, Direction dir, bool enemysTank) : dir(dir), enemysBullet(true) {
    switch(dir) {
        case RIGHT : case LEFT :
            shape.setSize(sf::Vector2f(20.f, 3.f));
            break;
        case UP : case DOWN:
            shape.setSize(sf::Vector2f(3.f, 20.f));
    }
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setPosition(position);
    setVelocity();
}

void Bullet :: changeVelocity(Direction dir, int mirrorType) {
    switch(dir) {
        case RIGHT : case LEFT :
            shape.setSize(sf::Vector2f(20.f, 3.f));
            break;
        case UP : case DOWN:
            shape.setSize(sf::Vector2f(3.f, 20.f));
    }
    switch(dir) {
        case UP:
            velocity = sf::Vector2f(0.f, -1.f); dir = UP; break;
        case DOWN:
            velocity = sf::Vector2f(0.f, 1.f); dir = DOWN; break;
        case LEFT:
            velocity = sf::Vector2f(-1.f, 0.f); dir = LEFT;
            /*if(mirrorType == 1) {

                shape.setPosition(sf::Vector2f(shape.getPosition().x - 10.f, shape.getPosition().y - 10.f));
                
            }*/
            break;
        case RIGHT:
            velocity = sf::Vector2f(1.f, 0.f);
            dir = RIGHT;
            /*if(mirrorType == 1 || mirrorType == 3) {
                shape.setPosition(sf::Vector2f(shape.getPosition().x + 10.f, shape.getPosition().y - 10.f));
            } */
            break;
    }
}

void Bullet :: setVelocity() {
    switch(dir) {
        case UP:
            velocity = sf::Vector2f(0.f, -1.f); break;
        case DOWN:
            velocity = sf::Vector2f(0.f, 1.f); break;
        case LEFT:
            velocity = sf::Vector2f(-1.f, 0.f); break;
        case RIGHT:
            velocity = sf::Vector2f(1.f, 0.f); break;
    }
}

void Bullet :: setVelocity(Direction bulletDir) {
    switch(bulletDir) {
        case RIGHT : case LEFT :
            shape.setSize(sf::Vector2f(20.f, 3.f));
            break;
        case UP : case DOWN:
            shape.setSize(sf::Vector2f(3.f, 20.f));
    }
    switch(bulletDir) {
        case UP:
            velocity = sf::Vector2f(0.f, -1.f); break;
        case DOWN:
            velocity = sf::Vector2f(0.f, 1.f); break;
        case LEFT:
            velocity = sf::Vector2f(-1.f, 0.f); break;
        case RIGHT:
            velocity = sf::Vector2f(1.f, 0.f); break;
    }
}

void Bullet :: setTransparentColor() {
    shape.setFillColor(sf::Color(255, 0, 0, 0));
}

void Bullet :: returnOldColor() {
    shape.setFillColor(sf::Color::Red);
}

void Bullet :: update(sf::Time updateTime) {
    if(enemysBullet) {
        shape.move(velocity * static_cast<float>(updateTime.asMilliseconds()));
        
    }
    shape.move(velocity * static_cast<float>(updateTime.asMilliseconds()/1.5));

}
 
sf::Vector2f Bullet :: getDimensions() const {
    return shape.getSize();
}

Direction Bullet :: getDirection() const {
    return dir;
}

void Bullet :: setPosition(float x, float y) { shape.setPosition(x, y); }


sf::Vector2f Bullet :: getPosition() const { return shape.getPosition(); }