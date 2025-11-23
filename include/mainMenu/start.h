#pragma once

#include <SFML/Graphics.hpp>
#include "resources.h"

class Start : public sf::Drawable {
private:
    sf::RectangleShape startButton;
    sf::Text startText;
    bool clicked = false;

public:
    Start(int menuWinSizeX, int menuWinSizeY) {
        startButton.setPosition(sf::Vector2f(static_cast<float>(menuWinSizeX) * 425 / 794, static_cast<float>(menuWinSizeY) * 512 / 800));
        startButton.setSize(sf::Vector2f(static_cast<float>(menuWinSizeX) * 344 / 794, static_cast<float>(menuWinSizeY) * 67 / 800));
        startButton.setFillColor(sf::Color::White);


        startText.setFont(globalFont);
        startText.setCharacterSize(24);
        startText.setFillColor(sf::Color::Black);

        float textWidth = startText.getLocalBounds().width;
        float textHeight = startText.getLocalBounds().height;
        startText.setPosition(
            startButton.getPosition().x + (startButton.getSize().x - textWidth) / 2.f,
            startButton.getPosition().y + (startButton.getSize().y - textHeight) / 2.f - 5.f
        );
        sf::Color rectColor = startButton.getFillColor();
        rectColor.a = 255;
        startButton.setFillColor(rectColor);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(startButton);
        target.draw(startText);
    }

    void handleClick(const sf::Vector2f& mousePos) {
        if (startButton.getGlobalBounds().contains(mousePos)) {
            clicked = true;
        }
    }

    sf::RectangleShape& getButton() {
        return startButton;
    }

    

    bool wasClicked() const {
        return clicked;
    }
};
