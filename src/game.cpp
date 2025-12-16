#include "../include/game.h"
#include "../include/playerInteraction.h"
#include "../include/loadSound.h"
#include "../include/gameLost.h"

#include "../include/mainMenu/resources.h"


#include "../include/gameWonWindow.h"


MainGame::MainGame(int screenSizeX, int screenSizeY, int constwindowSizeX, int constwindowSizeY, int level) :
    window(std::make_unique<sf::RenderWindow>(
        sf::VideoMode(screenSizeX, screenSizeY),
        "LaserTank",
        sf::Style::Close | sf::Style::Resize)),
    tileMap(level),
    player(tileMap.getPlayerPositionX(), tileMap.getPlayerPositionY(), constwindowSizeX, constwindowSizeY),
    windowSizeX(constwindowSizeX),
    windowSizeY(constwindowSizeY),
    bullets(0, nullptr),
    padding(screenSizeX - constwindowSizeX, screenSizeY, constwindowSizeX)
{
    this -> screenSizeX = screenSizeX;
    this -> screenSizeY = screenSizeY;
    padding.padding.setPosition(constwindowSizeX*1.f, 0.f);
    padding.initPadding();
    loadMirrorBubbleSound("sounds/bubble.mp3");
    loadHitWallSoundBuffer("sounds/hitTankSoundAndBrickSound.mp3");
    auto image = sf::Image{};
    std::string iconPath = "Images/tank.png";

    if (!image.loadFromFile(iconPath)) {
        throw std::runtime_error("Failed to load icon from: " + iconPath);
    }
    backClicked = false;

    window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    int playerPosX = tileMap.getPlayerPositionX();
    int playerPosY = tileMap.getPlayerPositionY();
    sf::Vector2i playerPos = sf::Vector2i(playerPosX, playerPosY);
    player.setGridPosition(playerPos);
    window->setFramerateLimit(60);

    loadGameOverSound("sounds/game-over.mp3");
    loadWonSound("sounds/ENDLEV.mp3");



    playerPositions.push_back(std::make_pair<int, int>(static_cast<unsigned>(playerPosX), static_cast<unsigned>(playerPosY)));
    padding.initPadding();

    backButtonAbsPosX = 527.f;
    backButtonAbsPosY = 270.f;

    backButtonSizeX = 158.f;
    backButtonSizeY = 40.f;

}

void MainGame::run() {

    bool waterToggle = false;
    int framesTank = 0; // 0, 1, 2
    int frames = 0;
    sf::Clock deltaClock;
    while (window->isOpen()) {
        undoMoveEndGame = false;
        if (!gameEnd()) {

            float dt = deltaClock.restart().asSeconds();

            if (windowEnableRepeatMovement.getElapsedTime() >= windowEnableRepMovTime) {
                isRepeatMovEnabled = true;
                windowEnableRepeatMovement.restart();
            }

            

            sf::Time time = waterFramesClock.getElapsedTime();
            if (time >= updateFramesWaterTile) {
                waterToggle = !waterToggle;

                tileMap.switchFramesWaterTiles(waterToggle ? 1 : 0);
                waterFramesClock.restart();
            }
            sf::Time timeTank = enemyTankFramesClock.getElapsedTime();
            if(timeTank >= updateFramesEnemyTank) {
                
                framesTank = (framesTank + 1) % 3;
                 
                tileMap.switchFramesEnemyTank(framesTank);
                enemyTankFramesClock.restart();
            }

            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::MouseButtonPressed &&
                        event.mouseButton.button == sf::Mouse::Left)
                {
                        sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);

                        if (padding.undoButton.getGlobalBounds().contains(mouse))
                            undoMove();
                            
                        if(padding.backButton.getGlobalBounds().contains(mouse)) {
                            backClicked = true;
                            window -> close();
                        }
                

                }
                if (event.type == sf::Event::Closed) {

                    window->close();
                    isWindowClosed = true;
                }
                if (event.type == sf::Event::KeyPressed) {
                    
                    std::cout << "here" << std::endl;
                    handleInput();
                }
            }

            PlayerInteraction interaction (windowSizeX, windowSizeY, player, tileMap, sf::Keyboard::Unknown);
            interaction.handlePlayerTileSignal();

            tileMap.updateTransportTracks();
            tileMap.updateWaterTiles();

            

            update();
            render();
        }
        else {

            sf::Time gameEndShownTime = sf::seconds(3);
            sf::Clock clock;
            while (window->isOpen() && !undoMoveEndGame) {
                sf::Event event;
                while (window->pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window->close();
                }

                window->clear(sf::Color::White);
                if (gameWon()) {
                    levelWonSound.play();
                    GameWonWindow gameWonWindow;
                    gameWonWindow.run();
                    initials = gameWonWindow.initialsField.initials;
                    window -> close();
                }
                else if (gameLost()) {

                    gameOverSound.play();
                    GameLostWindow gameLostWindow;
                    gameLostWindow.loop();
                    if(gameLostWindow.getUndoMove()) {
                        killPlayer = false;
                        player.setDontKillPlayer();
                        undoMove();
                        undoMoveEndGame = true;
                        for(int i = 0; i < bullets.size(); i++) {
                            bullets.erase(bullets.begin() + i);
                        }
                    }
                    if(gameLostWindow.getRestartGame()) {
                        window->close();
                    }
                }
                window->display();
            }
        }
    }
}




void MainGame::undoMove() {
    if (movesPlayed > 0) {

        movesPlayed--;
    }

    if (!tankMovedOrBulletShot.empty()) {
        std::string lastAction = tankMovedOrBulletShot.back();

        if (lastAction == "bullet shot") {
                
            if (!mapStates.empty()) {
                
                tileMap.undoMove(&mapStates.back());
                
                mapStates.pop_back();
            }
        } 
        else if (lastAction == "tank moved") {
            
            if (!player.getPlayerStates().empty()) {
                player.setGridPosition(player.getPlayerStates().back().playerPos);
                player.setDir(player.getPlayerStates().back().dir);
                player.getPlayerStates().pop_back();
                
            }
        }
            
        tankMovedOrBulletShot.pop_back();
    }
    padding.movesText.setString("Moves:"  + std::to_string(movesPlayed));
}

bool MainGame::playerKilledByEnemy() {
    for (const auto& bullet : bullets) {


        bool killed = (static_cast<int>(bullet->getPosition().x / tileSize == player.getGridPosition().x) && static_cast<int>(bullet->getPosition().y / tileSize == player.getGridPosition().y));
        if (killed) {
            return true;
        }
    }
    return false;
}

bool MainGame::shouldEnemyFireBullet() {
    for (int i = 0; i < tileMap.getTileMap().size(); i++) {
        for (int j = 0; j < tileMap.getTileMap()[i].size(); j++) {
            if (tileMap.getTileMap()[i][j]) {

                if (tileMap.getTileMap()[i][j]->killPlayer(
                    tileMap.getTileMap(),
                    player.getGridPosition().x * tileSize,
                    player.getGridPosition().y * tileSize)) {

                    tileMap.getTileMap()[i][j]->fireBullet();
                    bullets.push_back(tileMap.getTileMap()[i][j]->getBullet());
                    bulletFired = true;
                    coordXKillerTank = j;
                    coordYKillerTank = i;

                    isRepeatMovEnabled = false;
                    window->setKeyRepeatEnabled(false);
                    return true;
                }
            }
        }
    }
    return false;
}

void MainGame::handleInput() {
    sf::Keyboard::Key pressedKey = sf::Keyboard::Unknown;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Up;
        tankMovedOrBulletShot.push_back("tank moved");

    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Down;
        tankMovedOrBulletShot.push_back("tank moved");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Left;
        tankMovedOrBulletShot.push_back("tank moved");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        movesPlayed++;
        pressedKey = sf::Keyboard::Right;
        tankMovedOrBulletShot.push_back("tank moved");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        pressedKey = sf::Keyboard::Space;
        
            movesPlayed++;
            tankMovedOrBulletShot.push_back("bullet shot");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U) && 
        (!mapStates.empty() || !player.getPlayerStates().empty())) 
    {
    
        
        undoMove();

        return;
    }

    if (pressedKey != sf::Keyboard::Unknown) {
        if(tankMovedOrBulletShot.back() == "bullet shot")  {

            mapStates.push_back(tileMap.getMapState());
        }
        PlayerInteraction interaction (windowSizeX, windowSizeY, player, tileMap, pressedKey);
        interaction.handleMovement ();
        interaction.handleFire ();  
        interaction.handlePlayerTileSignal ();
        padding.movesText.setString("Moves: " + std::to_string(movesPlayed));
    }

    if (tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() && returnFromTrack && pressedKey != sf::Keyboard::Unknown) {
        returnFromTrack = false;
    }
}


void MainGame::update() {

    

    if (isRepeatMovEnabled) {
        window->setKeyRepeatEnabled(true);
    }

    if (bullets.size() == 0) {
        bulletFired = false;
    }

    if (player.getBullet() != nullptr) {

        BulletInteraction bulletInteract(windowSizeX, windowSizeY, player, tileMap);
        bulletInteract.interact();
    }
    
    if (bullets.size() == 0 || (player.getGridPosition().x != coordXKillerTank && player.getGridPosition().y != coordYKillerTank)) {

        shouldEnemyFireBullet();
    }


    if (bullets.size() > 0) {
        const sf::Time updateInterval = sf::seconds(1.f / 60.f);
        for (int i = 0; i < bullets.size(); i++) {
            bullets[i]->update(updateInterval);
        }
       
        
        for (int i = 0; i < bullets.size(); i++) {

            sf::Vector2f bulletPos = bullets[i]->getPosition();
            if (static_cast<int>(bulletPos.x) / tileSize == player.getGridPosition().x &&
                static_cast<int>(bulletPos.y) / tileSize == player.getGridPosition().y) {
                killPlayer = true;
                
            }
            if (bullets[i]->getPosition().x < 0 || bullets[i]->getPosition().x > 512 ||
                bullets[i]->getPosition().y < 0 || bullets[i]->getPosition().y > windowSizeY) {
                bullets.erase(bullets.begin() + i);
            }
        }

    }
}






void MainGame::render() {
    window->clear(sf::Color::White);
    window->draw(tileMap);
    window->draw(player);
    for (int i = 0; i < bullets.size(); i++) {
        window->draw(*bullets[i]);
    }
    window -> draw(padding);

    window->display();
}

bool  MainGame::gameLost() {
    return player.getShouldKillPlayer() == true || killPlayer;
}

bool MainGame::gameWon() {
    return tileMap.getTileMapInt()[player.getGridPosition().y][player.getGridPosition().x] == 10;
}

bool MainGame::gameEnd() {
    return gameLost() || gameWon();
}

bool MainGame::getWindowClosedState() const {
    return isWindowClosed;
}