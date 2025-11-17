#include "../include/game.h"
#include "../include/playerInteraction.h"
#include "../include/loadSound.h"
#include "../include/gameLost.h"


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
    padding(sf::Vector2f(static_cast<float>(screenSizeX - constwindowSizeX), static_cast<float>(screenSizeY)))
{
    this -> screenSizeX = screenSizeX;
    this -> screenSizeY = screenSizeY;
    padding.setPosition(constwindowSizeX*1.f, 0.f);
    initPadding();
    loadMirrorBubbleSound("sounds/bubble.mp3");
    loadHitWallSoundBuffer("sounds/hitTankSoundAndBrickSound.mp3");
    auto image = sf::Image{};
    std::string iconPath = "Images/tank.png";

    if (!image.loadFromFile(iconPath)) {
        throw std::runtime_error("Failed to load icon from: " + iconPath);
    }

    window->setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
    int playerPosX = tileMap.getPlayerPositionX();
    int playerPosY = tileMap.getPlayerPositionY();
    sf::Vector2i playerPos = sf::Vector2i(playerPosX, playerPosY);
    player.setGridPosition(playerPos);
    window->setFramerateLimit(60);
    loadGoblet();
    loadGameOverFont();



    playerPositions.push_back(std::make_pair<int, int>(static_cast<unsigned>(playerPosX), static_cast<unsigned>(playerPosY)));


}


void MainGame::run() {

    bool waterToggle = false;
    while (window->isOpen()) {
        undoMoveEndGame = false;
        if (!gameEnd()) {

            if (windowEnableRepeatMovement.getElapsedTime() >= windowEnableRepMovTime) {
                isRepeatMovEnabled = true;
                windowEnableRepeatMovement.restart();
            }

            

            sf::Time time = waterFramesClock.getElapsedTime();
            if (time >= updateFramesWaterTile) {
                waterToggle = !waterToggle; // prebaci frame
                tileMap.switchFramesWaterTiles(waterToggle ? 1 : 0);
                waterFramesClock.restart();
            }

            sf::Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {

                    window->close();
                    isWindowClosed = true;
                }
                if (event.type == sf::Event::KeyPressed && (!tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() || returnFromTrack) && (tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() || !bulletFired)) {

                    handleInput();
                }
            }

            tileMap.updateTransportTracks();
            tileMap.updateWaterTiles();

            

            if (tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() && !returnFromTrack) {
                handlTransportableTrack(player.getGridPosition().y, player.getGridPosition().x);

            }
            if (!tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack()) {
                returnFromTrack = false;
            }

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
                    drawGoblet();
                }
                else if (gameLost()) {
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
        movesPlayed++;
        pressedKey = sf::Keyboard::Space;
        tankMovedOrBulletShot.push_back("bullet shot");
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U) && 
        (!mapStates.empty() || !player.getPlayerStates().empty())) 
    {
    
        
        undoMove();

        return;
    }

    if (pressedKey != sf::Keyboard::Unknown) {
        if(tankMovedOrBulletShot.back() == "bullet shot") 
            mapStates.push_back(tileMap.getMapState());

        PlayerInteraction playerInteraction(windowSizeX, windowSizeY, player, tileMap, pressedKey);
        playerInteraction.handleMovement();
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

void MainGame :: drawPadding() {
    window->draw(topPad);
    window->draw(bottomPad);
    window->draw(leftPad);
    window->draw(rightPad);
}

void MainGame :: initPadding() {
    sf::Color borderColor(180, 180, 180); // light gray

    // Top padding
    topPad.setSize(sf::Vector2f(screenSizeX, (screenSizeY - windowSizeY) / 2.f));
    topPad.setFillColor(borderColor);
    topPad.setPosition(0, 0);

    // Bottom padding
    bottomPad.setSize(sf::Vector2f(screenSizeX, (screenSizeY - windowSizeY) / 2.f));
    bottomPad.setFillColor(borderColor);
    bottomPad.setPosition(0,windowSizeY + (screenSizeY - windowSizeY) / 2.f);

    // Left padding
    leftPad.setSize(sf::Vector2f((screenSizeX - windowSizeX) / 2.f, windowSizeY));
    leftPad.setFillColor(borderColor);
    leftPad.setPosition(0, (screenSizeY - windowSizeY) / 2.f);

    // Right padding
    rightPad.setSize(sf::Vector2f((screenSizeX - windowSizeX) / 2.f, windowSizeY));
    rightPad.setFillColor(borderColor);
    rightPad.setPosition(windowSizeX + (screenSizeX - windowSizeX) / 2.f,
                         (screenSizeY - windowSizeY) / 2.f);
}


void MainGame::render() {
    window->clear(sf::Color::White);
    window->draw(tileMap);
    window->draw(player);
    for (int i = 0; i < bullets.size(); i++) {
        window->draw(*bullets[i]);
    }
    if (gameLost()) {
        window->draw(gameOverText);
    }

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

void MainGame::drawGoblet() {

    window->draw(gobletSprite);
}

void MainGame::loadGoblet() {
    try {
        if (!gobletTexture.loadFromFile("Images/goblet.png")) {
            throw std::runtime_error("Cannot load goblet image");
        }

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return;
    }
    gobletSprite.setTexture(gobletTexture);
    gobletSprite.setOrigin(gobletSprite.getLocalBounds().width / 2, gobletSprite.getLocalBounds().height / 2);
    gobletSprite.setPosition(sf::Vector2f(static_cast<float>(windowSizeX) / 2, static_cast<float>(windowSizeY) / 2));
}
void MainGame::loadGameOverFont() {
    try {
        if (!weirdFont.loadFromFile("Fonts/Creepster-Regular.ttf")) {
            throw std::runtime_error("Cannot load weird font");
        }

        gameOverText.setFont(weirdFont);

        gameOverText.setString("GAME OVER");

        gameOverText.setCharacterSize(80);

        gameOverText.setFillColor(sf::Color::Red);

        gameOverText.setStyle(sf::Text::Bold | sf::Text::Italic);

        sf::FloatRect textBounds = gameOverText.getLocalBounds();

        gameOverText.setOrigin(textBounds.left + textBounds.width / 2.f,
            textBounds.top + textBounds.height / 2.f);

        gameOverText.setPosition(static_cast<float>(windowSizeX) / 2,
            static_cast<float>(windowSizeY) / 2);

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


void MainGame::handlTransportableTrack(int y, int x) {
    if (returnFromTrack) return;

    int dx = 0, dy = 0;
    if (tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() == 1) {
        dx = -1;
    }
    else if (tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() == 2) {
        dx = 1;
    }
    else if (tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() == 3) {
        dy = -1;
    }
    else if (tileMap.getTileMap()[player.getGridPosition().y][player.getGridPosition().x]->isTransportTrack() == 4) {
        dy = 1;
    }
    if (player.validMove(x + dx, y + dy)) {

        if (tileMap.getTileMap()[y + dy][x + dx]->isTileMovableBlock() ||
            tileMap.getTileMap()[y + dy][x + dx]->isBulletDestroyable() ||
            tileMap.getTileMap()[y + dy][x + dx]->isMirror1() ||
            tileMap.getTileMap()[y + dy][x + dx]->isMirror2() ||
            tileMap.getTileMap()[y + dy][x + dx]->isMirror3() ||
            tileMap.getTileMap()[y + dy][x + dx]->isMirror4() ||
            tileMap.getTileMap()[y + dy][x + dx]->isTank()) {
            returnFromTrack = true;
            return;
        }
        int currGridCoordX = x;
        int currGridCoordY = y;

        returnFromTrack = false;

        player.setGridPosition(sf::Vector2i(x + dx, y + dy));

    }
}

bool MainGame::getWindowClosedState() const {
    return isWindowClosed;
}