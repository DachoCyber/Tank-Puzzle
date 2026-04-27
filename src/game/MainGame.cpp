#include "mainGame.h"
#include "../../include/loadSound.h"
#include "../../include/mainMenu/resources.h"

MainGame::MainGame(int screenSizeX, int screenSizeY,
                   int constwindowSizeX, int constwindowSizeY, int level)
    : window(std::make_unique<sf::RenderWindow>(
          sf::VideoMode(screenSizeX, screenSizeY),
          "LaserTank", sf::Style::Close | sf::Style::Resize)),
      windowSizeX(constwindowSizeX), windowSizeY(constwindowSizeY),
      screenSizeX(screenSizeX),      screenSizeY(screenSizeY),
      tileMap(level),
      player(tileMap.getPlayerPositionX(), tileMap.getPlayerPositionY(),
             constwindowSizeX, constwindowSizeY),
      padding(screenSizeX - constwindowSizeX, screenSizeY, constwindowSizeX),

      bulletManager(player, constwindowSizeY, 32),
      enemyManager (player, tileMap, bulletManager, *window),
      undoManager  (player, tileMap, movesPlayed),
      inputHandler (player, tileMap, undoManager, bulletManager,
                    padding, constwindowSizeX, constwindowSizeY,
                    movesPlayed, returnFromTrack, isRepeatMovEnabled),
      stateChecker (player, tileMap, bulletManager),
      renderer     (*window, tileMap, player, bulletManager, padding),
      gameLoop     (*window, player, tileMap, padding,
                    bulletManager, enemyManager, inputHandler,
                    undoManager, stateChecker, renderer,
                    constwindowSizeX, constwindowSizeY,
                    backClicked, isWindowClosed, initials
                    )
{
    
    padding.padding.setPosition(constwindowSizeX * 1.f, 0.f);
    padding.initPadding();
    
    sf::Image image;
    if (!image.loadFromFile("Images/tank.png"))
        throw std::runtime_error("Failed to load icon: Images/tank.png");
    
    window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    player.setGridPosition({tileMap.getPlayerPositionX(),
                             tileMap.getPlayerPositionY()});
    window->setFramerateLimit(60);
    
    loadMirrorBubbleSound("sounds/bubble.mp3");
    loadHitWallSoundBuffer("sounds/hitTankSoundAndBrickSound.mp3");
    loadGameOverSound("sounds/game-over.mp3");
    loadWonSound("sounds/ENDLEV.mp3");
}

void MainGame::run() {
    gameLoop.run();
}