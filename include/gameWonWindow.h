#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

struct InitialsField {
    std::string initials;
    sf::Text initialsText;
};

class GameWonWindow {
public:
    std::unique_ptr<sf::RenderWindow> window;

    sf::Text gameWonText;
    sf::Sprite winSprite;
    sf::Texture winTexture;

    InitialsField initialsField;
    sf::Text initialsConstText;

    sf::RectangleShape inputBox;
    sf::RectangleShape okButton;
    sf::Text okButtonText;

    bool mainWinShouldClose = false;


    void handleInput();
    void render();
    void run();


    GameWonWindow();
};
