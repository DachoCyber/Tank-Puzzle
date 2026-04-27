#include "MainMenu.h"


MainMenu::MainMenu(int levelCount)
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

    if (!backgroundImageTexture.loadFromImage(backgroundImage)) {
        throw std::runtime_error("Failed to load texture!");
    }

    backgroundImageSprite.setTexture(backgroundImageTexture);

    sf::Vector2u imageSize = backgroundImageTexture.getSize();
    float scaleX = 794.f / imageSize.x;
    float scaleY = 800.f / imageSize.y;
    backgroundImageSprite.setScale(scaleX, scaleY);

    detectButtonColor();

    levels.level = 1;
}

void MainMenu::setChosenLevel(int lvl)
{
    level = lvl;
}

bool MainMenu::getWinClose() const
{
    return !winClose;
}

int MainMenu::getChosenLevel() const
{
    return level;
}

void MainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(backgroundImageSprite);
    target.draw(levels);
    target.draw(start);
}

void MainMenu::run()
{
    sf::Time  updateLevelTextTime = sf::seconds(0.25f);
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
                break;
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos =
                    window->mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                levels.handleClick(mousePos);
                start.handleClick(mousePos);
                editor.button.handleClick(mousePos);
            }
            
        }
        if (winClose) {
            break;
        }

        sf::Vector2f mousePos =
            window->mapPixelToCoords(sf::Mouse::getPosition(*window));

        bool hoveringStartNow  = start.getButton().getGlobalBounds().contains(mousePos);
        bool hoveringEditorNow = editor.getButton().getGlobalBounds().contains(mousePos);
        bool hoveringLevelsNow = levels.getButton().getGlobalBounds().contains(mousePos);

        updateHover(hoverStart,  hoveringStartNow,  dt);
        updateHover(hoverEditor, hoveringEditorNow, dt);
        updateHover(hoverLevels, hoveringLevelsNow, dt);

        if (start.wasClicked() && levels.level == -1) {
            if (levelTextClock.getElapsedTime().asSeconds() >=
                updateLevelTextTime.asSeconds() / 2)
            {
                levels.setTextColor(sf::Color::Red);
            }
            if (levelTextClock.getElapsedTime().asSeconds() >=
                updateLevelTextTime.asSeconds())
            {
                levels.setTextColor(sf::Color::Black);
                levelTextClock.restart();
            }
        }

        if (start.wasClicked() && levels.level != -1) {
            level = levels.level;
            window->close();
            winClose = true;
        }

        if (editor.button.wasClicked()) {
            editor.handleClick();
        }

        render();
    }
}

void MainMenu::detectButtonColor()
{
    long sumR = 0, sumG = 0, sumB = 0;
    long count = 0;

    sf::Vector2u size = backgroundImage.getSize();

    for (unsigned y = 0; y < size.y; y += 2) {
        for (unsigned x = 0; x < size.x; x += 2) {
            sf::Color c = backgroundImage.getPixel(x, y);

            if (c.r > c.g && c.g > c.b) {
                sumR += c.r;
                sumG += c.g;
                sumB += c.b;
                ++count;
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

void MainMenu::updateHover(HoverState& h, bool isHovering, float dt)
{
    const float fadeSpeed = 6.f;
    float target = isHovering ? 1.f : 0.f;

    if (h.alpha < target)
        h.alpha = std::min(target, h.alpha + fadeSpeed * dt);
    else if (h.alpha > target)
        h.alpha = std::max(target, h.alpha - fadeSpeed * dt);
}

void MainMenu::drawGlowForButton(const sf::RectangleShape& button, const HoverState& h)
{
    if (h.alpha <= 0.f)
        return;

    float t     = hoverAnimClock.getElapsedTime().asSeconds();
    float pulse = 0.5f * (1.f + std::sin(t * 4.f));

    float intensity = h.alpha * pulse;

    sf::RectangleShape glow(button.getSize());
    glow.setPosition(button.getPosition());

    sf::Color glowColor = baseButtonColor;
    glowColor.r = std::min<sf::Uint8>(255, glowColor.r + 60);
    glowColor.g = std::min<sf::Uint8>(255, glowColor.g + 40);
    glowColor.b = std::min<sf::Uint8>(255, glowColor.b + 20);
    glowColor.a = static_cast<sf::Uint8>(120 * intensity);

    glow.setFillColor(glowColor);

    sf::Color outline = glowColor;
    outline.a = static_cast<sf::Uint8>(200 * intensity);
    glow.setOutlineThickness(3.f);
    glow.setOutlineColor(outline);

    window->draw(glow);
}

void MainMenu::render()
{
    window->clear(sf::Color::Black);

    window->draw(backgroundImageSprite);

    drawGlowForButton(start.getButton(),  hoverStart);
    drawGlowForButton(editor.getButton(), hoverEditor);
    drawGlowForButton(levels.getButton(), hoverLevels);

    window->display();
}