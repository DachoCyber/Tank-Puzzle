#pragma once

#include <SFML/Graphics.hpp>
#include "resources.h"

class EditorButton : public sf::Drawable {
private:
    sf::Text editorText;
    bool clicked = false;

    bool editorWinClose;

public:
    sf::RectangleShape editorButton;
    EditorButton(int menuWinSizeX, int menuWinSizeY, bool editorWinClose) {
        editorButton.setPosition(sf::Vector2f(static_cast<float>(menuWinSizeX) * 425 / 794, static_cast<float>(menuWinSizeY) * 690 / 800));
        editorButton.setSize(sf::Vector2f(static_cast<float>(menuWinSizeX) * 344 / 794, static_cast<float>(menuWinSizeY) * 67 / 800));
        editorButton.setFillColor(sf::Color::White);

        this->editorWinClose = editorWinClose;

        editorText.setFont(globalFont);
        editorText.setCharacterSize(24);
        editorText.setFillColor(sf::Color::Black);

        float textWidth = editorText.getLocalBounds().width;
        float textHeight = editorText.getLocalBounds().height;
        editorText.setPosition(
            editorButton.getPosition().x + (editorButton.getSize().x - textWidth) / 2.f,
            editorButton.getPosition().y + (editorButton.getSize().y - textHeight) / 2.f - 5.f
        );
        sf::Color rectColor = editorButton.getFillColor();
        rectColor.a = 255;

        editorButton.setFillColor(rectColor);
    }

    sf::RectangleShape& getButton() {
        return editorButton;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(editorButton);
        target.draw(editorText);
    }

    void handleClick(const sf::Vector2f& mousePos) {
        if (editorButton.getGlobalBounds().contains(mousePos)) {
            clicked = true;
        }
    }

    bool wasClicked() const {
        return clicked;
    }
};
