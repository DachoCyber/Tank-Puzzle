#include "../include/tileFactory.h"
#include "../include/textures.h"
#include "../include/tileCode.h"

std::unique_ptr<Tile> TileFactory :: constructTile(int code, int x, int y, int tileSize) {
    std::unique_ptr<Tile> tile;
    switch (code) {
        case PLAYER_START:
            tile = std::make_unique<WalkableGround>(x * tileSize, y * tileSize, walkableTexture);
            break;
        case WALKABLE_GROUND:
            tile = std::make_unique<WalkableGround>(x * tileSize, y * tileSize, walkableTexture);
            break;
        case DESTRUCTIBLE_BLOCK:
            tile = std::make_unique<DestructibleBlock>(x * tileSize, y * tileSize, destructibleTexture);
            break;
        case MIRROR_1:
            tile = std::make_unique<Mirror1Tile>(x * tileSize, y * tileSize, mirror1Texture);
            break;
        case MIRROR_2:
            tile = std::make_unique<Mirror2Tile>(x * tileSize, y * tileSize, mirror2Texture);
            break;
        case MIRROR_3:
            tile = std::make_unique<Mirror3Tile>(x * tileSize, y * tileSize, mirror3Texture);
            break;
        case MIRROR_4:
            tile = std::make_unique<Mirror4Tile>(x * tileSize, y * tileSize, mirror4Texture);
            break;
        case WATER:
            tile = std::make_unique<WaterTile>(x * tileSize, y * tileSize, waterTileTexture, waterSecondFrameTexture);
            break;
        case MOVABLE_BLOCK:
            tile = std::make_unique<MovableBlock>(x * tileSize, y * tileSize, movableBlockTexture);
            break;
        case FLAG:
            tile = std::make_unique<Flag>(x * tileSize, y * tileSize, flagTexture);
            break;
        case UNDESTRUCTABLE_BLOCK:
            tile = std::make_unique<UndestructableBlock>(x * tileSize, y * tileSize, undestructableBlockTex);
            break;
        case ENEMY_TANK_LEFT:
            tile = std::make_unique<EnemyTank1>(x * tileSize, y * tileSize, 0, EnemyTank1LeftTexture);
            break;
        case ENEMY_TANK_RIGHT:
            tile = std::make_unique<EnemyTank1>(x * tileSize, y * tileSize, 1, EnemyTank1RightTexture);
            break;
        case ENEMY_TANK_UP:
            tile = std::make_unique<EnemyTank1>(x * tileSize, y * tileSize, 2, EnemyTank1UpTexture);
            break;
        case ENEMY_TANK_DOWN:
            tile = std::make_unique<EnemyTank1>(x * tileSize, y * tileSize, 3, EnemyTank1DownTexture);
            break;
        case DESTROYED_TANK_LEFT:
            tile = std::make_unique<DestroyedTank>(x * tileSize, y * tileSize, 0, destroyedTankLeftTexture);
            break;
        case DESTROYED_TANK_UP:
            tile = std::make_unique<DestroyedTank>(x * tileSize, y * tileSize, 1, destroyedTankUpTexture);
            break;
        case DESTROYED_TANK_DOWN:
            tile = std::make_unique<DestroyedTank>(x * tileSize, y * tileSize, 2, destroyedTankDownTexture);
            break;
        case DESTROYED_TANK_RIGHT:
            tile = std::make_unique<DestroyedTank>(x * tileSize, y * tileSize, 3, destroyedTankRightTexture);
            break;
        case TRANSPORT_TRACK_LEFT:
            tile = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, LEFT, transportTrackLeftTex);
            break;
        case TRANSPORT_TRACK_RIGHT:
            tile = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, RIGHT, transportTrackRightTex);
            break;
        case TRANSPORT_TRACK_UP:
            tile = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, UP, transportTrackUpTex);
            break;
        case TRANSPORT_TRACK_DOWN:
            tile = std::make_unique<TransportTrack>(x * tileSize, y * tileSize, DOWN, transportTrackDownTex);
            break;
        case TILE_IN_WATER:
            tile = std::make_unique<TileInWater>(x * tileSize, y * tileSize, tileInWaterTex);
            break;
        default:
            tile = nullptr;
            break;
        }

    return tile;
}