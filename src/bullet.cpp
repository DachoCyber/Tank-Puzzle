#pragma once
#include "../include/bullet.h"
#include <iostream>

namespace {
    sf::Vector2f getSize(Direction dir) {
        if (dir == LEFT || dir == RIGHT)
            return {20.f, 3.f};
        return {3.f, 20.f};
    }

    sf::Vector2f getVelocity(Direction dir) {
        switch (dir) {
            case UP:    return {0.f, -1.f};
            case DOWN:  return {0.f, 1.f};
            case LEFT:  return {-1.f, 0.f};
            case RIGHT: return {1.f, 0.f};
        }
        return {0.f, 0.f};
    }
}

Bullet::Bullet() : velocity(0.f, 0.f), dir(UP), enemysBullet(false) {
    shape.setSize({0.f, 0.f});
}

Bullet::Bullet(sf::Vector2f position, Direction d)
    : dir(d), enemysBullet(false) {
    shape.setSize(getSize(dir));
    shape.setFillColor(sf::Color(159, 43, 104));
    shape.setOutlineThickness(1.f);
    shape.setPosition(position);
    velocity = getVelocity(dir);
}

Bullet::Bullet(sf::Vector2f position, Direction d, bool enemy)
    : dir(d), enemysBullet(enemy) {
    shape.setSize(getSize(dir));
    shape.setFillColor(sf::Color::Red);
    shape.setOutlineThickness(1.f);
    shape.setPosition(position);
    velocity = getVelocity(dir);
}

void Bullet::changeVelocity(Direction newDir, int /*mirrorType*/) {
    dir = newDir;
    shape.setSize(getSize(dir));
    velocity = getVelocity(dir);
}

void Bullet::setVelocity() {
    velocity = getVelocity(dir);
}

void Bullet::setVelocity(Direction newDir) {
    dir = newDir;
    shape.setSize(getSize(dir));
    velocity = getVelocity(dir);
}

void Bullet::setTransparentColor() {
    shape.setFillColor(sf::Color(255, 0, 0, 0));
}

void Bullet::returnOldColor() {
    shape.setFillColor(sf::Color::Red);
}

void Bullet::update(sf::Time dt) {
    float factor = enemysBullet ? 1.f : (1.f / 1.5f);
    shape.move(velocity * static_cast<float>(dt.asMilliseconds()) * factor);
}

sf::Vector2f Bullet::getDimensions() const {
    return shape.getSize();
}

Direction Bullet::getDirection() const {
    return dir;
}

void Bullet::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

sf::Vector2f Bullet::getPosition() const {
    return shape.getPosition();
}
