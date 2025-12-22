#include "../include/tile.h"

void Tile::setAlpha(int alpha)
{
    sf::Color c = sprite.getColor();
    c.a = static_cast<sf::Uint8>(alpha);
    sprite.setColor(c);
}
