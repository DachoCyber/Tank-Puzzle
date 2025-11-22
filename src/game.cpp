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
    int framesTank = 0; // 0, 1, 2
    int frames = 0;
    sf::Clock deltaClock;
    while (window->isOpen()) {
        undoMoveEndGame = false;
        if (!gameEnd()) {

            float dt = deltaClock.restart().asSeconds();
    
            updateHUD(dt);
            blinkTimer += dt;

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

                        if (undoButton.getGlobalBounds().contains(mouse))
                            undoMove();
                }
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
                    GameWonWindow gameWonWindow;
                    gameWonWindow.run();
                    initials = gameWonWindow.initialsField.initials;
                    window -> close();
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
    movesText.setString("Moves:"  + std::to_string(movesPlayed));
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
        if(tankMovedOrBulletShot.back() == "bullet shot") 
            mapStates.push_back(tileMap.getMapState());

        PlayerInteraction playerInteraction(windowSizeX, windowSizeY, player, tileMap, pressedKey);
        playerInteraction.handleMovement();
        movesText.setString("Moves: " + std::to_string(movesPlayed));
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

    if (fmod(blinkTimer, 1.0f) < 0.5f) {
        light1.setFillColor(sf::Color(255,50,50));
        light2.setFillColor(sf::Color(50,255,50));
    } else {
        light1.setFillColor(sf::Color(120,0,0));
        light2.setFillColor(sf::Color(0,120,0));
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

void MainGame::updateHUD(float dt)
{
    blinkTimer += dt;

    if (std::fmod(blinkTimer, 1.0f) < 0.5f) {
        light1.setFillColor(sf::Color(255, 50, 50));
        light2.setFillColor(sf::Color(50, 255, 50));
    } else {
        light1.setFillColor(sf::Color(120, 0, 0));
        light2.setFillColor(sf::Color(0, 120, 0));
    }
}


void MainGame :: drawPadding() {
    // Draw minimap content
    minimapTexture.clear(sf::Color(30,50,30));

    // Example: draw player (scaled down)
    sf::CircleShape p(3);
    p.setFillColor(sf::Color::Yellow);
    p.setPosition(tileMap.getPlayerPositionX() / 10.0f,
                tileMap.getPlayerPositionY() / 10.0f);

    minimapTexture.draw(p);
    minimapTexture.display();

    // Draw HUD elements
    window->draw(rightPad);
    window->draw(borderLine);

    for (auto& c : hudCorners) window->draw(c);

    window->draw(titleText);
    window->draw(movesText);
    window->draw(controlsText);

    window->draw(iconUp);
    window->draw(iconDown);
    window->draw(iconLeft);
    window->draw(iconRight);

    window->draw(undoButton);
    window->draw(undoText);

    window->draw(minimapFrame);
    window->draw(minimapSprite);

}
void MainGame::initPadding()
{
    float hudX = windowSizeX;       // 512
    float hudWidth = screenSizeX - windowSizeX; // 188
    float hudY = 0;
    float hudHeight = windowSizeY; // 512

    // ============ HUD BACKGROUND ============
    rightPad.setSize(sf::Vector2f(hudWidth, hudHeight));
    rightPad.setFillColor(sf::Color(30, 50, 30));
    rightPad.setPosition(hudX, hudY);

    // ============ BORDER LINE ============
    borderLine.setSize(sf::Vector2f(3, hudHeight));
    borderLine.setFillColor(sf::Color(70, 120, 70));
    borderLine.setPosition(hudX - 3, 0);

    // ============ CLEAN CORNERS ============
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

    // ============ TEXT ============

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

    // ============ ICON POSITIONS ============
    float ix = hudX + 15;
    float iy = 150;

    iconUp.setPosition(ix, iy);
    iconRight.setPosition(ix, iy + 40);
    iconDown.setPosition(ix, iy + 80);
    iconLeft.setPosition(ix, iy + 120);

    // ============ UNDO BUTTON ============
    undoButton.setSize(sf::Vector2f(hudWidth - 30, 40));
    undoButton.setFillColor(sf::Color(0, 100, 0));
    undoButton.setPosition(hudX + 15, iy + 180);

    undoText.setFont(font);
    undoText.setCharacterSize(20);
    undoText.setFillColor(sf::Color(220, 255, 220));
    undoText.setString("Undo Move (U)");
    undoText.setPosition(undoButton.getPosition().x + 10,
                         undoButton.getPosition().y + 8);

    // ============ MINI-MAP ============
    minimapTexture.create(140, 140);

    minimapFrame.setSize(sf::Vector2f(140, 140));
    minimapFrame.setFillColor(sf::Color(20, 40, 20));
    minimapFrame.setOutlineColor(sf::Color(90, 140, 90));
    minimapFrame.setOutlineThickness(2);
    minimapFrame.setPosition(hudX + 15, undoButton.getPosition().y + 70);

    minimapSprite.setTexture(minimapTexture.getTexture());
    minimapSprite.setPosition(minimapFrame.getPosition());

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
    drawPadding();

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
            static_cast<float>(windowSizeY) / 4);

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