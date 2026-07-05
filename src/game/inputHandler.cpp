#include "inputHandler.h"
#include "../include/playerInteraction.h"
#include "../include/paddingGame.h"
#include "../include/tileCode.h"

InputHandler::InputHandler(Tank& player, Map& tileMap,
                           UndoManager& undoManager, BulletManager& bulletManager,
                           PaddingGame& padding,
                           int windowSizeX, int windowSizeY,
                           int& movesPlayed,
                           bool& returnFromTrack,
                           bool& isRepeatMovEnabled)
    : player(player), tileMap(tileMap),
      undoManager(undoManager), bulletManager(bulletManager),
      padding(padding),
      windowSizeX(windowSizeX), windowSizeY(windowSizeY),
      movesPlayed(movesPlayed),
      returnFromTrack(returnFromTrack),
      isRepeatMovEnabled(isRepeatMovEnabled) {}

bool InputHandler::isTransportTrack(int code) const {
    return tileIsTransportTrack(code);
}

void InputHandler::handleInput() {
    sf::Keyboard::Key pressedKey = sf::Keyboard::Unknown;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Up;
        undoManager.pushAction("tank moved");
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Down;
        undoManager.pushAction("tank moved");
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Left;
        undoManager.pushAction("tank moved");
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Right;
        undoManager.pushAction("tank moved");
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Space;
        undoManager.pushAction("bullet shot");
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U) && undoManager.canUndo()) {
        undoManager.undoMove();
        padding.movesText.setString("Moves: " + std::to_string(movesPlayed));
        return;
    }

    if (pressedKey == sf::Keyboard::Unknown)
        return;

    PlayerInteraction interaction(windowSizeX, windowSizeY, player, tileMap, pressedKey);
    interaction.handlePlayerTileSignal();
    interaction.handleMovement();
    interaction.handleFire();

    const auto& actions = undoManager.getActions();
    if (!actions.empty() && actions.back() == "bullet shot" && interaction.getBulletFired()) {
        undoManager.pushMapState(tileMap.getMapState());
    } else {
        if (!interaction.getPlayerMoved() && !interaction.getPlayerTurned()) {
            movesPlayed--;
            undoManager.getActions().pop_back();
        }
    }

    padding.movesText.setString("Moves: " + std::to_string(movesPlayed));

    int tileCode = tileMap.getTileMap()
        [player.getGridPosition().y][player.getGridPosition().x]->code();

    if (isTransportTrack(tileCode) && returnFromTrack)
        returnFromTrack = false;
}