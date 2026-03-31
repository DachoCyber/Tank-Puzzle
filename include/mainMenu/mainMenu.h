#pragma once

#include "level.h"
#include "quit.h"
#include "start.h"
#include "editor.h"
#include <memory>
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <cmath> // za sin, abs

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

    bool winClose;

    sf::Image  backgroundImage;
    sf::Image  originalBackgroundImage;
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

    sf::Clock hoverAnimClock; // za pulsiranje glow efekta

public:
    int level;

    MainMenu(int levelCount)
        : levels(menuWindowSizeX, menuWindowSizeY, levelCount),
          start(menuWindowSizeX, menuWindowSizeY),
          editor(menuWindowSizeX, menuWindowSizeY, editorWinClose),
          level(-1),
          window(std::make_unique<sf::RenderWindow>(
              sf::VideoMode(menuWindowSizeX, menuWindowSizeY),
              "Laser Tank",
              sf::Style::Default))
    {
        if (!backgroundImage.loadFromFile("Images/mainMenuImage.png")) {
            throw std::runtime_error("Failed to load main menu background image!");
        }

        originalBackgroundImage = backgroundImage;

        if (!backgroundImageTexture.loadFromImage(backgroundImage)) {
            throw std::runtime_error("Failed to load texture!");
        }

        backgroundImageSprite.setTexture(backgroundImageTexture);
        backgroundImageSprite.setScale(sf::Vector2f(794.f / 1024.f, 800.f / 1536.f));

        detectButtonColor(); // automatski nadji prosečnu boju dugmadi

        levels.level = 1;
    }
    void setChosenLevel(int level) {
        this -> level = level;
    }

    bool getWinClose() const {
        return winClose;
    }

    // --------------------------------------------------------
    // crtanje (ako negde radiš window->draw(mainMenu))
    // --------------------------------------------------------
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(backgroundImageSprite);
        // Napomena: hover efekat se crta u render()
        target.draw(levels);
        target.draw(start);
    }

    // --------------------------------------------------------
    // glavna petlja menija
    // --------------------------------------------------------
    void run() {
        sf::Time updateLevelTextTime = sf::seconds(0.25);
        sf::Clock levelTextClock;
        sf::Clock frameClock;

        window->setFramerateLimit(60);

        while (window->isOpen()) {
            float dt = frameClock.restart().asSeconds();

            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {

                    window->close();
                    winClose = true;
                }

                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {

                    sf::Vector2f mousePos =
                        window->mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                    levels.handleClick(mousePos);
                    start.handleClick(mousePos);
                    editor.button.handleClick(mousePos);
                }
            }

            sf::Vector2f mousePos =
                window->mapPixelToCoords(sf::Mouse::getPosition(*window));

            bool hoveringStartNow  = start.getButton().getGlobalBounds().contains(mousePos);
            bool hoveringEditorNow = editor.getButton().getGlobalBounds().contains(mousePos);
            bool hoveringLevelsNow = levels.getButton().getGlobalBounds().contains(mousePos);

            updateHover(hoverStart,  hoveringStartNow, dt);
            updateHover(hoverEditor, hoveringEditorNow, dt);
            updateHover(hoverLevels, hoveringLevelsNow, dt);

            // treperi tekst LEVEL kad nije izabran
            if (start.wasClicked() && levels.level == -1) {
                if (levelTextClock.getElapsedTime().asSeconds()
                    >= updateLevelTextTime.asSeconds() / 2) {
                    levels.setTextColor(sf::Color::Red);
                }
                if (levelTextClock.getElapsedTime().asSeconds()
                    >= updateLevelTextTime.asSeconds()) {
                    levels.setTextColor(sf::Color::Black);
                    levelTextClock.restart();
                }
            }

            // start igre
            if (start.wasClicked() && levels.level != -1) {
                level = levels.level;
                window->close();
                break;
            }

            // editor
            if (editor.button.wasClicked()) {
                editor.handleClick();
            }

            render();
        }
    }

    // --------------------------------------------------------
    // AUTOMATSKA DETEKCIJA BOJE DUGMETA IZ POZADINE
    // --------------------------------------------------------
    void detectButtonColor() {
        long sumR = 0, sumG = 0, sumB = 0;
        long count = 0;

        sf::Vector2u size = backgroundImage.getSize();

        // uzimamo svaki drugi piksel radi brzine
        for (unsigned y = 0; y < size.y; y += 2) {
            for (unsigned x = 0; x < size.x; x += 2) {

                sf::Color c = backgroundImage.getPixel(x, y);

                // pixel-art braon pločice: R > G > B
                if (c.r > c.g && c.g > c.b) {
                    sumR += c.r;
                    sumG += c.g;
                    sumB += c.b;
                    count++;
                }
            }
        }

        if (count > 0) {
            baseButtonColor = sf::Color(
                static_cast<sf::Uint8>(sumR / count),
                static_cast<sf::Uint8>(sumG / count),
                static_cast<sf::Uint8>(sumB / count)
            );
        }
    }

    // --------------------------------------------------------
    // update fade-in / fade-out hover stanja
    // --------------------------------------------------------
    void updateHover(HoverState& h, bool isHovering, float dt) {
        const float fadeSpeed = 6.f; // što veći, brži fade

        float target = isHovering ? 1.f : 0.f;

        if (h.alpha < target) {
            h.alpha = std::min(target, h.alpha + fadeSpeed * dt);
        } else if (h.alpha > target) {
            h.alpha = std::max(target, h.alpha - fadeSpeed * dt);
        }
    }

    // --------------------------------------------------------
    // crtanje glow efekta za jedno dugme
    // --------------------------------------------------------
    void drawGlowForButton(const sf::RectangleShape& button, const HoverState& h) {
        if (h.alpha <= 0.f)
            return;

        // pulsiranje (0.5 .. 1.0)
        float t = hoverAnimClock.getElapsedTime().asSeconds();
        float pulse = 0.5f * (1.f + std::sin(t * 4.f)); // brz puls

        float intensity = h.alpha * pulse; // kombinacija fade i pulsa

        sf::RectangleShape glow(button.getSize());
        glow.setPosition(button.getPosition());

        sf::Color glowColor = baseButtonColor;
        // malo posvetli braon
        glowColor.r = std::min<sf::Uint8>(255, glowColor.r + 60);
        glowColor.g = std::min<sf::Uint8>(255, glowColor.g + 40);
        glowColor.b = std::min<sf::Uint8>(255, glowColor.b + 20);
        glowColor.a = static_cast<sf::Uint8>(120 * intensity);

        glow.setFillColor(glowColor);

        // "glow" okvir
        sf::Color outline = glowColor;
        outline.a = static_cast<sf::Uint8>(200 * intensity);
        glow.setOutlineThickness(3.f);
        glow.setOutlineColor(outline);

        window->draw(glow);
    }

    // --------------------------------------------------------
    // render svega
    // --------------------------------------------------------
    void render() {
        window->clear(sf::Color::Black);

        // 1) pozadina
        window->draw(backgroundImageSprite);

        // 2) glow efekti preko pozadine, ispod teksta
        drawGlowForButton(start.getButton(),  hoverStart);
        drawGlowForButton(editor.getButton(), hoverEditor);
        drawGlowForButton(levels.getButton(), hoverLevels);

        // ako Editor ima svoj draw, dodaj window->draw(editor);

        window->display();
    }

    int getChosenLevel() const {
        return level;
    }
};
