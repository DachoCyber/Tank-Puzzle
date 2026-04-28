#pragma once

#include "editorButton.h"

#include <sstream>
#include <ostream>
#include <filesystem>
#include <curl/curl.h>
#include <fstream>
#include <vector>
#include <memory>

#include "../walkableGround.h"
#include "../tile.h"
#include "../destructibleBlock.h"
#include "../mirror1Tile.h"
#include "../mirror2Tile.h"
#include "../mirror3Tile.h"
#include "../mirror4Tile.h"
#include "../waterTile.h"
#include "../movableBlock.h"
#include "../undestructableBlock.h"
#include "../tank1.h"
#include "../destroyedTank.h"
#include "../transportTrack.h"
#include "../tileInWater.h"
#include "../textures.h"
#include "../flag.h"

class Editor : public sf::Drawable {
public:
    bool editorWinClose = false;

    int tileSize = 32;

    EditorButton button;

    sf::Texture tankTex;
    sf::Texture tileInWaterTex;

    std::vector<std::vector<int>> tileMap;

    sf::Font tankInstructionTextFont;
    sf::Text tankInstructionText;

    sf::RectangleShape tankInstructionBackground;
    sf::RectangleShape tileInstructionBackground;

    sf::Font tileInstructionTextFont;
    sf::Text tileInstructionText;

    // Created lazily on first handleClick() — no window until user opens editor
    std::unique_ptr<sf::RenderWindow> window;

    // Sidebar sprites (member variables, properly initialized in cpp)
    sf::Sprite destructibleBlockSprite;
    sf::Sprite mirror1Sprite;
    sf::Sprite mirror2Sprite;
    sf::Sprite mirror3Sprite;
    sf::Sprite mirror4Sprite;
    sf::Sprite waterSprite;
    sf::Sprite MovableBlockSprite;
    sf::Sprite flagSprite;
    sf::Sprite undestructableBlockSprite;

    sf::Sprite tank1LeftSprite;
    sf::Sprite tank1RightSprite;
    sf::Sprite tank1UpSprite;
    sf::Sprite tank1DownSprite;

    sf::Sprite transportLeftSprite;
    sf::Sprite transportDownSprite;
    sf::Sprite transportUpSprite;
    sf::Sprite transportRightSprite;

    sf::Sprite tankSprite;

public:
    Editor(int menuWinSizeX, int menuWinSizeY, bool editorWinClose);

    sf::RectangleShape& getButton();
    std::vector<std::vector<int>> getTileMap();

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    bool wasClicked();
    void handleClick();
    void pushTileMapToServer(int levelIndex);

    sf::Vector2i getTileCoords(sf::RenderWindow& window);

    void placeOrRemoveWaterTile(
        int tileX, int tileY,
        std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,
        std::vector<std::vector<int>>& tileMap);

    void placeOrRemoveTank(
        int tileX, int tileY,
        int placeID, int removeID,
        std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,
        std::vector<std::vector<int>>& tileMap,
        Direction dir,
        sf::Texture& tileTexture);

    void placeOrRemoveTrack(
        int tileX, int tileY,
        int placeID, int removeID,
        std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,
        std::vector<std::vector<int>>& tileMap,
        Direction dir,
        sf::Texture& tileTexture);

    template<typename TileType>
    void placeOrRemoveTile(
        int tileX, int tileY,
        int placeID, int removeID,
        std::vector<std::vector<std::unique_ptr<Tile>>>& tiles,
        std::vector<std::vector<int>>& tileMap,
        sf::Texture& tileTexture)
    {
        if (dynamic_cast<TileType*>(tiles[tileY][tileX].get())) {
            tileMap[tileY][tileX] = removeID;
            tiles[tileY][tileX] = std::make_unique<WalkableGround>(tileX * tileSize, tileY * tileSize, walkableTexture);
        } else {
            tileMap[tileY][tileX] = placeID;
            if (placeID == 8) {
                tiles[tileY][tileX] = std::make_unique<WaterTile>(
                    tileX * tileSize, tileY * tileSize,
                    tileInWaterTex, waterSecondFrameTexture);
            } else {
                tiles[tileY][tileX] = std::make_unique<TileType>(
                    tileX * tileSize, tileY * tileSize, tileTexture);
            }
        }
    }

    sf::Sprite createSprite(sf::Texture& texture, int gridX, int gridY, int tileSize);

    std::string generateNextMapFilename(const std::string& folderPath);
    int extractLevelIndex(const std::string& levelName);
};