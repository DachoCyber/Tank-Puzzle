#include "gameWonWindow.h"
#include "mainMenu/resources.h"

GameWonWindow::GameWonWindow()
{
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(320, 230),
        "Game Won!",
        sf::Style::Titlebar | sf::Style::Close
    );

    window->setFramerateLimit(60);

    // ------------------- TITLE -------------------
    gameWonText.setFont(globalFont);
    gameWonText.setString("GAME WON!");
    gameWonText.setCharacterSize(26);
    gameWonText.setFillColor(sf::Color(40, 40, 40));
    gameWonText.setPosition(85.f, 20.f);

    
    initialsConstText.setFont(globalFont);
    initialsConstText.setString("Initials:");
    initialsConstText.setCharacterSize(22);
    initialsConstText.setFillColor(sf::Color(40, 40, 40));
    initialsConstText.setPosition(sf::Vector2f(100.f, 85.f));

    // ------------------- INPUT BOX -------------------
    inputBox.setSize(sf::Vector2f(180, 35));
    inputBox.setFillColor(sf::Color(240, 240, 240));
    inputBox.setOutlineColor(sf::Color(100, 100, 100));
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(70.f, 120.f);

    // Initials text inside the box
    initialsField.initialsText.setFont(globalFont);
    initialsField.initialsText.setCharacterSize(20);
    initialsField.initialsText.setFillColor(sf::Color::Black);
    initialsField.initialsText.setPosition(80.f, 125.f);
    initialsField.initialsText.setString("");

    // ------------------- OK BUTTON -------------------
    okButton.setSize(sf::Vector2f(80, 35));
    okButton.setPosition(120.f, 170.f);
    okButton.setFillColor(sf::Color(80, 150, 80)); // green button
    okButton.setOutlineThickness(2);
    okButton.setOutlineColor(sf::Color(60, 110, 60));

    okButtonText.setFont(globalFont);
    okButtonText.setCharacterSize(20);
    okButtonText.setString("OK");
    okButtonText.setFillColor(sf::Color::White);
    okButtonText.setPosition(okButton.getPosition().x + 25, okButton.getPosition().y + 4);
}

void GameWonWindow::run()
{
    while (window->isOpen())
    {
        handleInput();
        render();
    }
}

void GameWonWindow::handleInput()
{
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window->close();
            mainWinShouldClose = true;
        }

        // --- Typing initials ---
        if (event.type == sf::Event::TextEntered)
        {
            if (std::isalpha(event.text.unicode) && initialsField.initials.size() < 3)
            {
                char c = static_cast<char>(std::toupper(event.text.unicode));
                initialsField.initials.push_back(c);
                initialsField.initialsText.setString(initialsField.initials);
            }
            else if (event.text.unicode == 8 && !initialsField.initials.empty()) // backspace
            {
                initialsField.initials.pop_back();
                initialsField.initialsText.setString(initialsField.initials);
            }
        }

        // --- OK button click ---
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

            if (okButton.getGlobalBounds().contains(mouse))
            {
                window->close();
            }
        }
    }

    // --- Hover effect for OK button ---
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
    if (okButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
        okButton.setFillColor(sf::Color(100, 180, 100));     // lighter green
    else
        okButton.setFillColor(sf::Color(80, 150, 80));
}

void GameWonWindow::render()
{
    window->clear(sf::Color(220, 220, 220)); // soft gray background

    window->draw(winSprite);
    window->draw(gameWonText);

    window->draw(inputBox);
    window->draw(initialsField.initialsText);

    window->draw(okButton);
    window->draw(okButtonText);

    window->draw(initialsConstText);

    window->display();
}
