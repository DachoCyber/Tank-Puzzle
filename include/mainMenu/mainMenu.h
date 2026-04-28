#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <thread>
#include <atomic>

#include "editor.h"         // your existing editor header
#include "start.h"    // adjust includes to match your project
#include "level.h"

struct HoverState {
    float alpha = 0.f;
};

class MainMenu : public sf::Drawable {
public:
    explicit MainMenu(int levelCount);

    void run();

    bool getWinClose() const;
    int  getChosenLevel() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    static constexpr int menuWindowSizeX = 794;
    static constexpr int menuWindowSizeY = 800;

    std::unique_ptr<sf::RenderWindow> window;

    sf::Image           backgroundImage;
    sf::Texture         backgroundImageTexture;
    sf::Sprite          backgroundImageSprite;

    Level       levels;
    Start       start;
    Editor      editor;

    int  level      = -1;
    bool winClose   = false;
    bool editorWinClose = false;

    // ── Editor thread ─────────────────────────────────────────────────
    std::thread          editorThread;
    std::atomic<bool>    editorRunning { false };

    // ── Hover / glow ──────────────────────────────────────────────────
    mutable HoverState hoverStart;
    mutable HoverState hoverEditor;
    mutable HoverState hoverLevels;

    sf::Color           baseButtonColor { 180, 120, 60 };
    mutable sf::Clock   hoverAnimClock;

    void setChosenLevel(int lvl);
    void detectButtonColor();
    void updateHover(HoverState& h, bool isHovering, float dt);
    void drawGlowForButton(const sf::RectangleShape& button, const HoverState& h);
    void render();

    // Launch editor in its own thread (non-blocking)
    void launchEditor();
};