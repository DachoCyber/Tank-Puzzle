#include "editor.h"

// ─────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────
Editor::Editor(int menuWinSizeX, int menuWinSizeY, bool editorWinClose)
    : button(menuWinSizeX, menuWinSizeY, editorWinClose)
    // window se NE kreira ovde — kreira se tek u handleClick()
{
    if (!tankTex.loadFromFile("Images/tank.png"))
        throw std::runtime_error("Failed to load tank texture");

    if (!tileInstructionTextFont.loadFromFile("Fonts/arial.ttf"))
        throw std::runtime_error("Failed to load instruction font!");

    tankInstructionTextFont = tileInstructionTextFont;

    // ── Initialize sidebar sprites as MEMBER variables (not locals) ──
    destructibleBlockSprite   = createSprite(destructibleTexture,       16, 0, tileSize);
    mirror1Sprite             = createSprite(mirror1Texture,            17, 0, tileSize);
    mirror2Sprite             = createSprite(mirror2Texture,            18, 0, tileSize);
    mirror3Sprite             = createSprite(mirror3Texture,            19, 0, tileSize);
    mirror4Sprite             = createSprite(mirror4Texture,            20, 0, tileSize);
    waterSprite               = createSprite(waterTileTexture,          21, 0, tileSize);
    MovableBlockSprite        = createSprite(movableBlockTexture,       22, 0, tileSize);
    flagSprite                = createSprite(flagTexture,               17, 2, tileSize);
    undestructableBlockSprite = createSprite(undestructableBlockTex,    23, 0, tileSize);

    tank1LeftSprite           = createSprite(EnemyTank1LeftTexture,     16, 1, tileSize);
    tank1RightSprite          = createSprite(EnemyTank1RightTexture,    17, 1, tileSize);
    tank1UpSprite             = createSprite(EnemyTank1UpTexture,       18, 1, tileSize);
    tank1DownSprite           = createSprite(EnemyTank1DownTexture,     19, 1, tileSize);

    transportLeftSprite       = createSprite(transportTrackLeftTex,     20, 1, tileSize);
    transportDownSprite       = createSprite(transportTrackDownTex,     21, 1, tileSize);
    transportUpSprite         = createSprite(transportTrackUpTex,       22, 1, tileSize);
    transportRightSprite      = createSprite(transportTrackRightTex,    23, 1, tileSize);

    tankSprite.setTexture(tankTex);
    tankSprite.setScale(sf::Vector2f(0.65f, 0.65f));
    // window se kreira u handleClick() — ovde ga nema
}

// ─────────────────────────────────────────────
// Simple accessors
// ─────────────────────────────────────────────
sf::RectangleShape& Editor::getButton()                    { return button.editorButton; }
std::vector<std::vector<int>> Editor::getTileMap()         { return tileMap; }
void Editor::draw(sf::RenderTarget& t, sf::RenderStates s) const { t.draw(button); }
bool Editor::wasClicked()                                  { return button.wasClicked(); }

// ─────────────────────────────────────────────
// handleClick  (main editor loop)
// ─────────────────────────────────────────────
void Editor::handleClick()
{
    if (editorWinClose) return;

    // ── Kreira se prozor OVDE, tek kad korisnik klikne na editor ──────
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(256 + 512, 512), "Editor", sf::Style::Default);
    window->setFramerateLimit(60);

    // ── Instruction panel ──────────────────────────────────────────────
    sf::Sprite tankInstructionSprite(tankTex);
    tankInstructionSprite.setScale(sf::Vector2f(0.65f, 0.65f));

    sf::RectangleShape instructionPanel;
    instructionPanel.setFillColor(sf::Color::White);
    instructionPanel.setSize(sf::Vector2f(256, 200));
    instructionPanel.setPosition(sf::Vector2f(512, 96));

    tankInstructionSprite.setPosition(
        instructionPanel.getPosition().x + 16,
        instructionPanel.getPosition().y + 16);

    sf::Text pressLbText;
    pressLbText.setFont(tankInstructionTextFont);
    pressLbText.setString("Click mouse Right\n Button for tank");
    pressLbText.setCharacterSize(24);
    pressLbText.setFillColor(sf::Color::Black);
    pressLbText.setPosition(
        tankInstructionSprite.getPosition().x + tankInstructionSprite.getGlobalBounds().width + 2,
        tankInstructionSprite.getPosition().y);

    sf::Text pressRbText;
    pressRbText.setFont(tankInstructionTextFont);
    pressRbText.setString("Click mouse Left\n Button for tile ");
    pressRbText.setCharacterSize(24);
    pressRbText.setFillColor(sf::Color::Black);
    pressRbText.setPosition(
        pressLbText.getPosition().x,
        pressLbText.getPosition().y + pressLbText.getGlobalBounds().height + 50);

    // ── Tile grid ──────────────────────────────────────────────────────
    std::vector<std::vector<std::unique_ptr<Tile>>> tiles;
    tiles.resize(16);
    tileMap.resize(16);

    for (int y = 0; y < 16; ++y) {
        tiles[y].resize(16);
        tileMap[y].resize(16);
        for (int x = 0; x < 16; ++x) {
            tiles[y][x] = std::make_unique<WalkableGround>(x * tileSize, y * tileSize, walkableTexture);
            tileMap[y][x] = WALKABLE_GROUND;
        }
    }

    // ── State ──────────────────────────────────────────────────────────
    bool tankPlaced = false;
    bool flagPlaced = false;
    int  tankPosX   = 0;
    int  tankPosY   = 0;

    bool placingCode[20] = {};

    tileInstructionBackground.setFillColor(sf::Color::White);
    tileInstructionBackground.setPosition(sf::Vector2f(512, 128));
    tileInstructionBackground.setSize(sf::Vector2f(256, 64));

    tankInstructionBackground.setFillColor(sf::Color::White);
    tankInstructionBackground.setSize(sf::Vector2f(256, 64));
    tankInstructionBackground.setPosition(sf::Vector2f(512, 96));

    // ── Event loop ─────────────────────────────────────────────────────
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window->close();
                editorWinClose = true;
            }

            // ── Right click → place player tank ───────────────────────
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Right)
            {
                sf::Vector2i tileCoords = getTileCoords(*window);
                tankPosX = tileCoords.x;
                tankPosY = tileCoords.y;
                tankSprite.setPosition(
                    static_cast<float>(tankPosX * tileSize),
                    static_cast<float>(tankPosY * tileSize));
                tankPlaced = true;
            }

            // ── Left click ────────────────────────────────────────────
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f worldPos   = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
                sf::Vector2i tileCoords = getTileCoords(*window);
                int tileX = tileCoords.x;
                int tileY = tileCoords.y;
                bool inGrid = (tileX >= 0 && tileX < 16 && tileY >= 0 && tileY < 16);

                auto selectTool = [&](int code) {
                    for (int i = 0; i < 20; ++i) placingCode[i] = false;
                    placingCode[code] = true;
                };

                if      (destructibleBlockSprite.getGlobalBounds().contains(worldPos))        { selectTool(3);  }
                else if (mirror1Sprite.getGlobalBounds().contains(worldPos))                  { selectTool(4);  }
                else if (mirror2Sprite.getGlobalBounds().contains(worldPos))                  { selectTool(5);  }
                else if (mirror3Sprite.getGlobalBounds().contains(worldPos))                  { selectTool(6);  }
                else if (mirror4Sprite.getGlobalBounds().contains(worldPos))                  { selectTool(7);  }
                else if (waterSprite.getGlobalBounds().contains(worldPos))                    { selectTool(8);  }
                else if (MovableBlockSprite.getGlobalBounds().contains(worldPos))             { selectTool(9);  }
                else if (flagSprite.getGlobalBounds().contains(worldPos))                     { selectTool(10); flagPlaced = true; }
                else if (undestructableBlockSprite.getGlobalBounds().contains(worldPos))      { selectTool(11); }
                else if (tank1LeftSprite.getGlobalBounds().contains(worldPos))                { selectTool(12); }
                else if (tank1RightSprite.getGlobalBounds().contains(worldPos))               { selectTool(13); }
                else if (tank1UpSprite.getGlobalBounds().contains(worldPos))                  { selectTool(14); }
                else if (tank1DownSprite.getGlobalBounds().contains(worldPos))                { selectTool(15); }
                else if (transportDownSprite.getGlobalBounds().contains(worldPos))            { selectTool(16); }
                else if (transportUpSprite.getGlobalBounds().contains(worldPos))              { selectTool(17); }
                else if (transportLeftSprite.getGlobalBounds().contains(worldPos))            { selectTool(18); }
                else if (transportRightSprite.getGlobalBounds().contains(worldPos))           { selectTool(19); }
                else if (inGrid) {
                    if      (placingCode[3])  placeOrRemoveTile<DestructibleBlock> (tileX, tileY, DESTRUCTIBLE_BLOCK,   WALKABLE_GROUND, tiles, tileMap, destructibleTexture);
                    else if (placingCode[4])  placeOrRemoveTile<Mirror1Tile>       (tileX, tileY, MIRROR_1,             WALKABLE_GROUND, tiles, tileMap, mirror1Texture);
                    else if (placingCode[5])  placeOrRemoveTile<Mirror2Tile>       (tileX, tileY, MIRROR_2,             WALKABLE_GROUND, tiles, tileMap, mirror2Texture);
                    else if (placingCode[6])  placeOrRemoveTile<Mirror3Tile>       (tileX, tileY, MIRROR_3,             WALKABLE_GROUND, tiles, tileMap, mirror3Texture);
                    else if (placingCode[7])  placeOrRemoveTile<Mirror4Tile>       (tileX, tileY, MIRROR_4,             WALKABLE_GROUND, tiles, tileMap, mirror4Texture);
                    else if (placingCode[8])  placeOrRemoveWaterTile               (tileX, tileY,       tiles, tileMap);
                    else if (placingCode[9])  placeOrRemoveTile<MovableBlock>      (tileX, tileY, MOVABLE_BLOCK,        WALKABLE_GROUND, tiles, tileMap, movableBlockTexture);
                    else if (placingCode[10]) placeOrRemoveTile<Flag>              (tileX, tileY, FLAG,                 WALKABLE_GROUND, tiles, tileMap, flagTexture);
                    else if (placingCode[11]) placeOrRemoveTile<UndestructableBlock>(tileX, tileY, UNDESTRUCTABLE_BLOCK, WALKABLE_GROUND, tiles, tileMap, undestructableBlockTex);
                    else if (placingCode[12]) placeOrRemoveTank (tileX, tileY, ENEMY_TANK_LEFT,  WALKABLE_GROUND, tiles, tileMap, LEFT,  EnemyTank1LeftTexture);
                    else if (placingCode[13]) placeOrRemoveTank (tileX, tileY, ENEMY_TANK_RIGHT, WALKABLE_GROUND, tiles, tileMap, RIGHT, EnemyTank1RightTexture);
                    else if (placingCode[14]) placeOrRemoveTank (tileX, tileY, ENEMY_TANK_UP,    WALKABLE_GROUND, tiles, tileMap, UP,    EnemyTank1UpTexture);
                    else if (placingCode[15]) placeOrRemoveTank (tileX, tileY, ENEMY_TANK_DOWN,  WALKABLE_GROUND, tiles, tileMap, DOWN,  EnemyTank1DownTexture);
                    else if (placingCode[16]) placeOrRemoveTrack(tileX, tileY, TRANSPORT_TRACK_DOWN,  WALKABLE_GROUND, tiles, tileMap, DOWN,  transportTrackDownTex);
                    else if (placingCode[17]) placeOrRemoveTrack(tileX, tileY, TRANSPORT_TRACK_UP,    WALKABLE_GROUND, tiles, tileMap, UP,    transportTrackUpTex);
                    else if (placingCode[18]) placeOrRemoveTrack(tileX, tileY, TRANSPORT_TRACK_LEFT,  WALKABLE_GROUND, tiles, tileMap, LEFT,  transportTrackLeftTex);
                    else if (placingCode[19]) placeOrRemoveTrack(tileX, tileY, TRANSPORT_TRACK_RIGHT, WALKABLE_GROUND, tiles, tileMap, RIGHT, transportTrackRightTex);
                }
            }
        }

        // ── Render ────────────────────────────────────────────────────
        window->clear();

        for (int y = 0; y < 16; ++y)
            for (int x = 0; x < 16; ++x)
                window->draw(*tiles[y][x]);

        window->draw(destructibleBlockSprite);
        window->draw(mirror1Sprite);
        window->draw(mirror2Sprite);
        window->draw(mirror3Sprite);
        window->draw(mirror4Sprite);
        window->draw(waterSprite);
        window->draw(MovableBlockSprite);
        window->draw(flagSprite);
        window->draw(undestructableBlockSprite);
        window->draw(tank1LeftSprite);
        window->draw(tank1DownSprite);
        window->draw(tank1UpSprite);
        window->draw(tank1RightSprite);
        window->draw(transportDownSprite);
        window->draw(transportLeftSprite);
        window->draw(transportRightSprite);
        window->draw(transportUpSprite);
        window->draw(instructionPanel);
        window->draw(tankInstructionSprite);
        window->draw(pressLbText);
        window->draw(pressRbText);

        if (tankPlaced)
            window->draw(tankSprite);

        window->display();
    }

    // Oslobodi prozor čim se zatvori — spreman za sledeće otvaranje
    window.reset();

    // ── Save map after window closes ──────────────────────────────────
    if (tankPlaced && flagPlaced) {
        const int width  = 16;
        const int height = 16;

        std::string folder    = "maps/";
        std::string levelName = generateNextMapFilename(folder);
        if (levelName.find(".tmx") == std::string::npos)
            levelName += ".tmx";

        std::ofstream file(folder + levelName);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing!\n";
            return;
        }

        int levelIndex = extractLevelIndex(levelName);

        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<map version=\"1.0\" tiledversion=\"1.9.2\" orientation=\"orthogonal\""
                " renderorder=\"right-down\" "
                "width=\"" << width << "\" height=\"" << height << "\""
                " tilewidth=\"32\" tileheight=\"32\" infinite=\"0\">\n";
        file << "  <tileset firstgid=\"1\" name=\"tileset\" tilewidth=\"32\""
                " tileheight=\"32\" tilecount=\"10\" columns=\"10\">\n";
        file << "    <image source=\"tileset.png\" width=\"320\" height=\"32\"/>\n";
        file << "  </tileset>\n";
        file << "  <layer name=\"Tile Layer 1\" width=\"" << width
             << "\" height=\"" << height << "\">\n";
        file << "    <data encoding=\"csv\">\n";

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                file << ((y == tankPosY && x == tankPosX) ? 0 : tileMap[y][x]);
                if (x < width - 1) file << ',';
            }
            file << (y < height - 1 ? ",\n" : "\n");
        }

        file << "    </data>\n  </layer>\n</map>\n";
        file.close();

        pushTileMapToServer(levelIndex);
    }
}

// ─────────────────────────────────────────────
// pushTileMapToServer
// ─────────────────────────────────────────────
void Editor::pushTileMapToServer(int levelIndex)
{
    CURL* curl = curl_easy_init();
    if (!curl) { std::cerr << "Failed to init curl\n"; return; }

    std::string fileName = "maps/map" + std::to_string(levelIndex) + ".tmx";
    if (!std::filesystem::exists(fileName)) {
        std::cerr << "File does not exist: " << fileName << "\n";
        curl_easy_cleanup(curl);
        return;
    }

    std::string url = "https://alas.matf.bg.ac.rs/~mr22033/levels/upload.php";

    curl_mime*     form  = curl_mime_init(curl);
    curl_mimepart* field = curl_mime_addpart(form);
    curl_mime_name(field, "file");
    curl_mime_filedata(field, fileName.c_str());

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL,           url.c_str());
    curl_easy_setopt(curl, CURLOPT_MIMEPOST,      form);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,     &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,       30L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE,       1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
    else {
        long code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
    }

    curl_mime_free(form);
    curl_easy_cleanup(curl);
}

// ─────────────────────────────────────────────
// Static callback
// ─────────────────────────────────────────────
size_t Editor::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ─────────────────────────────────────────────
// Tile helpers
// ─────────────────────────────────────────────
sf::Vector2i Editor::getTileCoords(sf::RenderWindow& win)
{
    sf::Vector2i  mousePos = sf::Mouse::getPosition(win);
    sf::Vector2f  worldPos = win.mapPixelToCoords(mousePos);
    return { static_cast<int>(worldPos.x) / tileSize,
             static_cast<int>(worldPos.y) / tileSize };
}

void Editor::placeOrRemoveWaterTile(
    int tileX, int tileY,
    std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,
    std::vector<std::vector<int>>& tileMap)
{
    if (dynamic_cast<WaterTile*>(tiles[tileY][tileX].get())) {
        tileMap[tileY][tileX] = WALKABLE_GROUND;
        tiles[tileY][tileX] = std::make_unique<WalkableGround>(
            tileX * tileSize, tileY * tileSize, walkableTexture);
    } else {
        tileMap[tileY][tileX] = WATER;
        tiles[tileY][tileX] = std::make_unique<WaterTile>(
            tileX * tileSize, tileY * tileSize,
            waterTileTexture, waterSecondFrameTexture);
    }
}

void Editor::placeOrRemoveTank(
    int tileX, int tileY,
    int placeID, int removeID,
    std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,
    std::vector<std::vector<int>>& tileMap,
    Direction dir, sf::Texture& tileTexture)
{
    if (dynamic_cast<EnemyTank1*>(tiles[tileY][tileX].get())) {
        tileMap[tileY][tileX] = removeID;
        tiles[tileY][tileX] = std::make_unique<WalkableGround>(
            tileX * tileSize, tileY * tileSize, walkableTexture);
    } else {
        tileMap[tileY][tileX] = placeID;
        tiles[tileY][tileX] = std::make_unique<EnemyTank1>(
            tileX * tileSize, tileY * tileSize, dir, tileTexture);
    }
}

void Editor::placeOrRemoveTrack(
    int tileX, int tileY,
    int placeID, int removeID,
    std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,
    std::vector<std::vector<int>>& tileMap,
    Direction dir, sf::Texture& tileTexture)
{
    if (dynamic_cast<TransportTrack*>(tiles[tileY][tileX].get())) {
        tileMap[tileY][tileX] = removeID;
        tiles[tileY][tileX] = std::make_unique<WalkableGround>(
            tileX * tileSize, tileY * tileSize, walkableTexture);
    } else {
        tileMap[tileY][tileX] = placeID;
        tiles[tileY][tileX] = std::make_unique<TransportTrack>(
            tileX * tileSize, tileY * tileSize, dir, tileTexture);
    }
}

// ─────────────────────────────────────────────
// Sprite factory
// ─────────────────────────────────────────────
sf::Sprite Editor::createSprite(sf::Texture& texture, int gridX, int gridY, int sz)
{
    sf::Sprite sprite(texture);
    sprite.setScale(
        sz / sprite.getGlobalBounds().width,
        sz / sprite.getGlobalBounds().height);
    sprite.setPosition(
        static_cast<float>(gridX * sz),
        static_cast<float>(gridY * sz));
    return sprite;
}

// ─────────────────────────────────────────────
// Map filename helpers
// ─────────────────────────────────────────────
std::string Editor::generateNextMapFilename(const std::string& folderPath)
{
    namespace fs = std::filesystem;
    int maxIndex = 0;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) continue;
        std::string fn = entry.path().filename().string();
        if (fn.size() > 7 && fn.substr(0, 3) == "map" &&
            fn.substr(fn.size() - 4) == ".tmx")
        {
            try {
                int idx = std::stoi(fn.substr(3, fn.size() - 7));
                if (idx > maxIndex) maxIndex = idx;
            } catch (...) {}
        }
    }
    return "map" + std::to_string(maxIndex + 1) + ".tmx";
}

int Editor::extractLevelIndex(const std::string& levelName)
{
    if (levelName.size() <= 7 ||
        levelName.substr(0, 3) != "map" ||
        levelName.substr(levelName.size() - 4) != ".tmx")
        throw std::invalid_argument("Invalid level name format");

    return std::stoi(levelName.substr(3, levelName.size() - 7));
}