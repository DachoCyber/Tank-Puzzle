#include "../include/paddingGame.h"

void PaddingGame::initPadding()
{
    float hudX = 512;      
    float hudWidth = 188;
    float hudY = 0;
    float hudHeight = 512;

    rightPad.setSize(sf::Vector2f(hudWidth, hudHeight));
    rightPad.setFillColor(sf::Color(30, 50, 30));
    rightPad.setPosition(hudX, hudY);

    borderLine.setSize(sf::Vector2f(3, hudHeight));
    borderLine.setFillColor(sf::Color(70, 120, 70));
    borderLine.setPosition(hudX - 3, 0);

    hudCorners.clear();
    float r = 12.f;

    sf::CircleShape topLeft(r);
    topLeft.setFillColor(sf::Color(30, 50, 30));
    topLeft.setPosition(hudX, 0);
    hudCorners.push_back(topLeft);

    sf::CircleShape topRight(r);
    topRight.setFillColor(sf::Color(30, 50, 30));
    topRight.setPosition(hudX + hudWidth - 2*r, 0);
    hudCorners.push_back(topRight);

    sf::CircleShape bottomLeft(r);
    bottomLeft.setFillColor(sf::Color(30, 50, 30));
    bottomLeft.setPosition(hudX, hudHeight - 2*r);
    hudCorners.push_back(bottomLeft);

    sf::CircleShape bottomRight(r);
    bottomRight.setFillColor(sf::Color(30, 50, 30));
    bottomRight.setPosition(hudX + hudWidth - 2*r, hudHeight - 2*r);
    hudCorners.push_back(bottomRight);

    font.loadFromFile("Fonts/arial.ttf");

    titleText.setFont(font);
    titleText.setCharacterSize(22);
    titleText.setFillColor(sf::Color(180, 255, 180));
    titleText.setString("CONTROL PANEL");
    titleText.setPosition(hudX + 15, 15);

    movesText.setFont(font);
    movesText.setCharacterSize(20);
    movesText.setFillColor(sf::Color(200, 255, 200));
    movesText.setString("Moves: 0");
    movesText.setPosition(hudX + 15, 60);

    controlsText.setFont(font);
    controlsText.setCharacterSize(18);
    controlsText.setFillColor(sf::Color(200, 255, 200));
    controlsText.setString(
        "Controls:\n"
        "        ^\n"
        "< - - + - - >\n"
        "        v\n"
        "\n"
        "U - Undo\n"
    );

    controlsText.setPosition(hudX + 15, 110);

    undoButton.setSize(sf::Vector2f(hudWidth - 30, 40));
    undoButton.setFillColor(sf::Color(0, 100, 0));
    undoButton.setPosition(hudX + 15, 150 + 180);

    undoText.setFont(font);
    undoText.setCharacterSize(20);
    undoText.setFillColor(sf::Color(220, 255, 220));
    undoText.setString("Undo Move (U)");
    undoText.setPosition(undoButton.getPosition().x + 10,
                         undoButton.getPosition().y + 8);

    minimapFrame.setSize(sf::Vector2f(140, 140));
    minimapFrame.setFillColor(sf::Color(20, 40, 20));
    minimapFrame.setOutlineColor(sf::Color(90, 140, 90));
    minimapFrame.setOutlineThickness(2);
    minimapFrame.setPosition(hudX + 15, undoButton.getPosition().y + 70);

    backButton.setSize(sf::Vector2f(hudWidth - 30, 40)); // 158, 40
    backButton.setPosition(sf::Vector2f(hudX + 15, 150 + 120));
    backButton.setFillColor(sf::Color(0, 100, 0));

    backButtonText.setFont(font);
    backButtonText.setString( "Main Menu");
    backButtonText.setCharacterSize(20);
    backButtonText.setPosition(backButton.getPosition().x + 10, 
                               backButton.getPosition().y + 8);
    backButtonText.setFillColor(sf::Color(220, 255, 220));

    // Draw minimap content
    minimapTexture.clear(sf::Color(30,50,30));

    minimapTexture.display();
}
void PaddingGame::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(rightPad);
    target.draw(borderLine);

    for (auto& c : hudCorners) target.draw(c);

    target.draw(titleText);
    target.draw(movesText);
    target.draw(controlsText);

    target.draw(undoButton);
    target.draw(undoText);

    target.draw(minimapFrame);
    target.draw(minimapSprite);
    target.draw(backButton);
    target.draw(backButtonText);

}
