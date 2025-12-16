#include <SFML/Graphics.hpp>

class PaddingGame : public sf::Drawable {

public:

    PaddingGame (float x, float y, float xpos) {
        padding.setSize({x, y});
        padding.setPosition({xpos, 0.f});
    }

    sf::RectangleShape padding;
    
    // HUD elements
    sf::RectangleShape rightPad;
    sf::RectangleShape leftPad, topPad, bottomPad;

    sf::Text titleText;
    sf::Text movesText;
    sf::Text controlsText;
    sf::Text undoText;
    sf::Font font;

    sf::RectangleShape undoButton;

    // Icons
    sf::Texture tankIconTexture;
    sf::Sprite iconUp, iconDown, iconLeft, iconRight;

    // Blinking lights
    sf::CircleShape light1, light2;
    float blinkTimer = 0.f;

    // Mini-map
    sf::RectangleShape minimapFrame;
    sf::RenderTexture minimapTexture;
    sf::Sprite minimapSprite;

    // For border
    sf::RectangleShape borderLine;
    std::vector<sf::CircleShape> hudCorners;

    sf::RectangleShape backButton;
    float backButtonAbsPosX, backButtonAbsPosY;
    float backButtonSizeX, backButtonSizeY;
    sf::Text backButtonText;
    
    void initPadding();
    void drawPadding();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};