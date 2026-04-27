#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& window, Map& tileMap,
                   Tank& player, BulletManager& bulletManager,
                   PaddingGame& padding)
    : window(window), tileMap(tileMap), player(player),
      bulletManager(bulletManager), padding(padding) {}

void Renderer::render() {
    window.clear(sf::Color::White);
    window.draw(tileMap);
    window.draw(player);
    for (auto* b : bulletManager.getBullets())
        window.draw(*b);
    window.draw(padding);
    window.display();
}