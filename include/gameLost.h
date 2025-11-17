#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class GameLostWindow {
public:
    GameLostWindow();
    ~GameLostWindow() {

    }

    bool getUndoMove() const {
        return undoMove;
    }
    bool getRestartGame() const {
        return restartGame;
    }

    void loop();
    void handleInput();
    void render();

    sf::RectangleShape undoMoveRect;
    sf::RectangleShape restartGameRect;

private:

    std::unique_ptr<sf::RenderWindow> gameLostWindow;
    sf::Font arialFont;

    bool undoMove = false;
    bool restartGame = false;

    sf::Text undoMoveText;
    sf::Text restartGameText;

    const std::string undoMoveString = "UNDO MOVE";
    const std::string restartGameString = "RESTART GAME";

    const int textSize = 10;

};