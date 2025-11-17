#include "../include/gameLost.h"

GameLostWindow::GameLostWindow() :
    undoMoveRect(sf::Vector2f(130, 30)),
    restartGameRect(sf::Vector2f(130, 30))
{
    gameLostWindow = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(150, 100),
        "",
        sf::Style::Default
    );

    if(!arialFont.loadFromFile("Fonts/arial.ttf")) {
        throw std::runtime_error("Failed to load game lost window text font!");
    }

    // Common button properties
    auto setupButton = [&](sf::RectangleShape& rect, float y) {
        rect.setPosition({10, y});
        rect.setFillColor(sf::Color(100, 100, 100));
        rect.setOutlineThickness(2);
        rect.setOutlineColor(sf::Color(30, 30, 30));
    };

    setupButton(undoMoveRect, 10);
    setupButton(restartGameRect, 55);

    // --- Text centering function ---
    auto centerText = [&](sf::Text &text, const std::string &str, sf::RectangleShape &rect) {
        text.setFont(arialFont);
        text.setString(str);
        text.setCharacterSize(textSize);
        text.setFillColor(sf::Color::White);

        sf::FloatRect tb = text.getLocalBounds();
        sf::FloatRect rb = rect.getGlobalBounds();

        text.setPosition(
            rb.left + (rb.width - tb.width) / 2.f - tb.left,
            rb.top + (rb.height - tb.height) / 2.f - tb.top
        );
    };

    centerText(undoMoveText, undoMoveString, undoMoveRect);
    centerText(restartGameText, restartGameString, restartGameRect);
}

void GameLostWindow::loop() {
    while(gameLostWindow->isOpen()) {
        handleInput();
        render();
    }
}

void GameLostWindow::handleInput() {
    sf::Event event;

    sf::Vector2i mouse = sf::Mouse::getPosition(*gameLostWindow);
    sf::Vector2f m(mouse.x, mouse.y);

    // Hover efekti
    auto applyHover = [&](sf::RectangleShape& rect) {
        if(rect.getGlobalBounds().contains(m))
            rect.setFillColor(sf::Color(140, 140, 140));
        else
            rect.setFillColor(sf::Color(100, 100, 100));
    };

    applyHover(undoMoveRect);
    applyHover(restartGameRect);

    while(gameLostWindow->pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            if(!undoMove && !restartGame)
                undoMove = true;
            gameLostWindow->close();
        }

        if(event.type == sf::Event::MouseButtonPressed &&
           event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            if(restartGameRect.getGlobalBounds().contains(mousePos)) {
                restartGame = true;
                gameLostWindow->close();
            }
            else if(undoMoveRect.getGlobalBounds().contains(mousePos)) {
                undoMove = true;
                gameLostWindow->close();
            }
        }
    }
}

void GameLostWindow::render() {
    // Soft background
    gameLostWindow->clear(sf::Color(70, 70, 70));

    // Draw order
    gameLostWindow->draw(undoMoveRect);
    gameLostWindow->draw(undoMoveText);
    gameLostWindow->draw(restartGameRect);
    gameLostWindow->draw(restartGameText);

    gameLostWindow->display();
}
