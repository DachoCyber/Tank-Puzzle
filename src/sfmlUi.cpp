#include "../include/sfmlScoreUI.h"
#include "../include/mainMenu/resources.h"
#include <algorithm>
#include <SFML/Graphics.hpp>

// TOP 3 BOJE
static const sf::Color GOLD   (255, 215,   0);
static const sf::Color SILVER (192, 192, 192);
static const sf::Color BRONZE (205, 127,  50);

void sfmlUi(std::string initials, int chosenLevel, int score) {

    // get players for that level
    std::vector<PlayerTable> players = getPlayerScores(chosenLevel);

    // Sort ascending by score
    std::sort(players.begin(), players.end(), [](const PlayerTable& a, const PlayerTable& b) {
        return a.score < b.score;
    });

    auto it = std::unique(players.begin(), players.end());

    // Erase the duplicate elements
    players.erase(it, players.end());

    // create window
    sf::RenderWindow window(sf::VideoMode(600, 400),
        "Score Table - Level " + std::to_string(chosenLevel));
    window.setFramerateLimit(60);

    // enable mouse wheel
    window.setMouseCursorVisible(true);

    // table layout
    const float margin = 20.f;
    const float headerH = 30.f;
    const float rowH = 28.f;
    const float colNameW = 300.f;
    const float colScoreW = 120.f;
    const float startX = margin;
    const float startY = margin;

    // prepare header texts
    sf::Text headerName("Name", globalFont, 18);
    headerName.setPosition(startX + 5, startY + 4);
    headerName.setFillColor(sf::Color::Black);

    sf::Text headerScore("Score", globalFont, 18);
    headerScore.setPosition(startX + colNameW + 5, startY + 4);
    headerScore.setFillColor(sf::Color::Black);

    // prepare rows text
    std::vector<sf::Text> nameTexts;
    std::vector<sf::Text> scoreTexts;

    for (size_t idx = 0; idx < players.size(); ++idx) {

        sf::Text tName(players[idx].name, globalFont, 16);
        tName.setFillColor(sf::Color::Black);
        nameTexts.push_back(tName);

        sf::Text tScore(std::to_string(players[idx].score), globalFont, 16);
        tScore.setFillColor(sf::Color::Black);
        scoreTexts.push_back(tScore);
    }

    // limit visible rows to fit window
    const int visibleRows =
        static_cast<int>((window.getSize().y - startY - headerH - margin) / rowH);

    int offset = 0; // scroll offset

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            // keyboard scroll
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down) {
                    if (offset + visibleRows < (int)players.size()) offset++;
                } else if (event.key.code == sf::Keyboard::Up) {
                    if (offset > 0) offset--;
                }
            }

            // MOUSE WHEEL SCROLL
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta < 0) {
                    // scroll down
                    if (offset + visibleRows < (int)players.size()) offset++;
                } else if (event.mouseWheelScroll.delta > 0) {
                    // scroll up
                    if (offset > 0) offset--;
                }
            }
        }

        window.clear(sf::Color(160, 32, 240));

        // draw header background
        sf::RectangleShape headerBg(sf::Vector2f(colNameW + colScoreW, headerH));
        headerBg.setPosition(startX, startY);
        headerBg.setFillColor(sf::Color(220, 220, 220));
        window.draw(headerBg);

        // draw header texts
        window.draw(headerName);
        window.draw(headerScore);

        // draw rows
        for (int r = 0; r < visibleRows; ++r) {

            int idx = offset + r;
            if (idx >= (int)players.size()) break;

            float y = startY + headerH + r * rowH;

            // highlight initials
            bool isInitials = (players[idx].name == initials);

            sf::RectangleShape rowBg(sf::Vector2f(colNameW + colScoreW, rowH));
            rowBg.setPosition(startX, y);

            // *** TOP 3 COLORING ***
            if (idx == 0)      rowBg.setFillColor(GOLD);
            else if (idx == 1) rowBg.setFillColor(SILVER);
            else if (idx == 2) rowBg.setFillColor(BRONZE);
            else if (isInitials)
                rowBg.setFillColor(sf::Color(180, 210, 250));  // highlighted player
            else
                rowBg.setFillColor(sf::Color(245, 245, 245));

            window.draw(rowBg);

            // dividing line
            sf::RectangleShape line(sf::Vector2f(colNameW + colScoreW, 1.f));
            line.setPosition(startX, y + rowH - 1);
            line.setFillColor(sf::Color(200, 200, 200));
            window.draw(line);

            // update text positions
            sf::Text nameT = nameTexts[idx];
            nameT.setPosition(startX + 5, y + 4);
            window.draw(nameT);

            sf::Text scoreT = scoreTexts[idx];
            scoreT.setPosition(startX + colNameW + 5, y + 4);
            window.draw(scoreT);
        }

        // instructions text
        sf::Text instr("Scroll mouse or Up/Down. Highlighted = " + initials,
            globalFont, 12);
        instr.setPosition(startX, window.getSize().y - margin);
        instr.setFillColor(sf::Color(80, 80, 80));
        window.draw(instr);

        window.display();
    }
}
