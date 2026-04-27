#include "gameLoop.h"
#include "../../include/playerInteraction.h"
#include "../../include/bulletInteraction.h"
#include "../../include/gameLost.h"
#include "../../include/gameWonWindow.h"
#include "../../include/loadSound.h"

GameLoop::GameLoop(sf::RenderWindow& window,
                   Tank& player, Map& tileMap, PaddingGame& padding,
                   BulletManager& bulletManager, EnemyManager& enemyManager,
                   InputHandler& inputHandler, UndoManager& undoManager,
                   GameStateChecker& stateChecker, Renderer& renderer,
                   int windowSizeX, int windowSizeY,
                   bool& backClicked, bool& isWindowClosed,
                   std::string& initials)

    : window(window), player(player), tileMap(tileMap), padding(padding),
      bulletManager(bulletManager), enemyManager(enemyManager),
      inputHandler(inputHandler), undoManager(undoManager),
      stateChecker(stateChecker), renderer(renderer),
      windowSizeX(windowSizeX), windowSizeY(windowSizeY),
      backClicked(backClicked), isWindowClosed(isWindowClosed),
      initials(initials) {}

void GameLoop::run() {
    while (window.isOpen()) {
        undoMoveEndGame = false;

        if (!stateChecker.gameEnd())
            processNormalFrame();
        else
            processEndGameFrame();
    }
}

void GameLoop::handleWindowEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            isWindowClosed = true;
        }
        if (event.type == sf::Event::KeyPressed)
            inputHandler.handleInput();

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(
                {event.mouseButton.x, event.mouseButton.y}, window.getView());

            if (padding.undoButton.getGlobalBounds().contains(mouse))
                undoManager.undoMove();

            if (padding.backButton.getGlobalBounds().contains(mouse)) {
                backClicked = true;
                window.close();
            }
        }
    }
}

void GameLoop::updateAnimations() {
    if (waterFramesClock.getElapsedTime() >= updateFramesWaterTile) {
        waterToggle = !waterToggle;
        tileMap.switchFramesWaterTiles(waterToggle ? 1 : 0);
        waterFramesClock.restart();
    }

    if (enemyTankFramesClock.getElapsedTime() >= updateFramesEnemyTank) {
        framesTank = (framesTank + 1) % 3;
        tileMap.switchFramesEnemyTank(framesTank);
        enemyTankFramesClock.restart();
    }
}

void GameLoop::processNormalFrame() {
    if (windowEnableRepeatMovement.getElapsedTime() >= windowEnableRepMovTime) {
        isRepeatMovEnabled = true;
        window.setKeyRepeatEnabled(true);
        windowEnableRepeatMovement.restart();
    }

    updateAnimations();
    handleWindowEvents();

    PlayerInteraction interaction(windowSizeX, windowSizeY, player, tileMap,
                                  sf::Keyboard::Unknown);
    interaction.handlePlayerTileSignal();
    tileMap.updateTransportTracks();

    if (player.getBullet() != nullptr) {
        BulletInteraction bulletInteract(windowSizeX, windowSizeY, player, tileMap);
        bulletInteract.interact();
    }

    auto& bm = bulletManager;
    if (!bm.hasBullets() ||
        (player.getGridPosition().x != bm.coordXKillerTank &&
         player.getGridPosition().y != bm.coordYKillerTank))
    {
        PlayerInteraction blocked(windowSizeX, windowSizeY, player, tileMap,
                                  sf::Keyboard::Unknown);
        blocked.setBlockInput(true);
        enemyManager.shouldEnemyFireBullet();
    }

    bulletManager.update();
    renderer.render();
}

void GameLoop::processEndGameFrame() {
    while (window.isOpen() && !undoMoveEndGame) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::White);

        if (stateChecker.gameWon()) {
            levelWonSound.play();
            GameWonWindow gameWonWindow;
            gameWonWindow.run();
            initials = gameWonWindow.initialsField.initials;
            window.close();
        } else if (stateChecker.gameLost()) {
            gameOverSound.play();
            GameLostWindow gameLostWindow;
            gameLostWindow.loop();

            if (gameLostWindow.getUndoMove()) {
                bulletManager.setKillPlayer(false);
                player.setDontKillPlayer();
                undoManager.undoMove();
                bulletManager.clearBullets();
                undoMoveEndGame = true;
            }
            if (gameLostWindow.getRestartGame())
                window.close();
        }
        window.display();
    }
}