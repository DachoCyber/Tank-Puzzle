#pragma once

#include "level.h"
#include "quit.h"
#include "start.h"
#include "editor.h"
#include <memory>
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <cmath>

class MainMenu : public sf::Drawable {
private:
    Level levels;
    Start start;
    Editor editor;
    static constexpr int menuWindowSizeX = 794;
    static constexpr int menuWindowSizeY = 800;
    std::vector<std::vector<int>> tileMap;
    std::unique_ptr<sf::RenderWindow> window;

    bool editorWinClose = false;
    bool winClose = false;

    sf::Image   backgroundImage;
    sf::Image   originalBackgroundImage;
    sf::Texture backgroundImageTexture;
    sf::Sprite  backgroundImageSprite;

    // automatski detektovana osnovna boja dugmeta
    sf::Color baseButtonColor{111, 70, 2};

    // stanje hovera (fade in/out)
    struct HoverState {
        float alpha = 0.f;   // 0 = nema efekta, 1 = pun efekat
    };

    HoverState hoverStart;
    HoverState hoverEditor;
    HoverState hoverLevels;

    sf::Clock hoverAnimClock;

    // helpers
    void detectButtonColor();
    void updateHover(HoverState& h, bool isHovering, float dt);
    void drawGlowForButton(const sf::RectangleShape& button, const HoverState& h);
    void render();

public:
    int level;

    explicit MainMenu(int levelCount);

    void setChosenLevel(int level);
    bool getWinClose() const;
    int  getChosenLevel() const;

    void run();

    // sf::Drawable interface
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};