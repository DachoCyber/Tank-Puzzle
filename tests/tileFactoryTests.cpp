#include "doctest.h"

#include "../include/tileFactory.h"
#include "../include/tileCode.h"

TEST_CASE("constructTile builds a walkable ground for the player start code") {
    auto tile = TileFactory::constructTile(PLAYER_START, 0, 0, 32);
    REQUIRE(tile != nullptr);
    CHECK(tile->code() == WALKABLE_GROUND);
}

TEST_CASE("constructTile returns a tile whose code matches the requested code") {
    int codes[] = {
        WALKABLE_GROUND,
        DESTRUCTIBLE_BLOCK,
        MIRROR_1,
        MIRROR_2,
        MIRROR_3,
        MIRROR_4,
        WATER,
        MOVABLE_BLOCK,
        FLAG,
        UNDESTRUCTABLE_BLOCK,
        ENEMY_TANK_LEFT,
        ENEMY_TANK_RIGHT,
        ENEMY_TANK_UP,
        ENEMY_TANK_DOWN,
        DESTROYED_TANK_LEFT,
        DESTROYED_TANK_UP,
        DESTROYED_TANK_DOWN,
        DESTROYED_TANK_RIGHT,
        TRANSPORT_TRACK_LEFT,
        TRANSPORT_TRACK_RIGHT,
        TRANSPORT_TRACK_UP,
        TRANSPORT_TRACK_DOWN,
        TILE_IN_WATER
    };

    for (int code : codes) {
        auto tile = TileFactory::constructTile(code, 2, 3, 32);
        REQUIRE(tile != nullptr);
        CHECK(tile->code() == code);
    }
}

TEST_CASE("constructTile places the tile at the scaled grid position") {
    auto tile = TileFactory::constructTile(WALKABLE_GROUND, 4, 5, 32);
    REQUIRE(tile != nullptr);
    CHECK(tile->code() == WALKABLE_GROUND);
}

TEST_CASE("constructTile returns nullptr for codes that are not tile types") {
    CHECK(TileFactory::constructTile(2, 0, 0, 32) == nullptr);
    CHECK(TileFactory::constructTile(24, 0, 0, 32) == nullptr);
    CHECK(TileFactory::constructTile(49, 0, 0, 32) == nullptr);
    CHECK(TileFactory::constructTile(99, 0, 0, 32) == nullptr);
    CHECK(TileFactory::constructTile(-1, 0, 0, 32) == nullptr);
}

TEST_CASE("enemy tank codes map onto the four consecutive facings") {
    CHECK(TileFactory::constructTile(ENEMY_TANK_LEFT,  0, 0, 32)->code()  == ENEMY_TANK_LEFT);
    CHECK(TileFactory::constructTile(ENEMY_TANK_RIGHT, 0, 0, 32)->code()  == ENEMY_TANK_RIGHT);
    CHECK(TileFactory::constructTile(ENEMY_TANK_UP,    0, 0, 32)->code()  == ENEMY_TANK_UP);
    CHECK(TileFactory::constructTile(ENEMY_TANK_DOWN,  0, 0, 32)->code()  == ENEMY_TANK_DOWN);
}

TEST_CASE("destroyed tank codes map onto the four consecutive facings") {
    CHECK(TileFactory::constructTile(DESTROYED_TANK_LEFT,  0, 0, 32)->code()  == DESTROYED_TANK_LEFT);
    CHECK(TileFactory::constructTile(DESTROYED_TANK_UP,    0, 0, 32)->code()  == DESTROYED_TANK_UP);
    CHECK(TileFactory::constructTile(DESTROYED_TANK_DOWN,  0, 0, 32)->code()  == DESTROYED_TANK_DOWN);
    CHECK(TileFactory::constructTile(DESTROYED_TANK_RIGHT, 0, 0, 32)->code()  == DESTROYED_TANK_RIGHT);
}

TEST_CASE("transport track codes map onto the four directions") {
    CHECK(TileFactory::constructTile(TRANSPORT_TRACK_LEFT,  0, 0, 32)->code() == TRANSPORT_TRACK_LEFT);
    CHECK(TileFactory::constructTile(TRANSPORT_TRACK_RIGHT, 0, 0, 32)->code() == TRANSPORT_TRACK_RIGHT);
    CHECK(TileFactory::constructTile(TRANSPORT_TRACK_UP,    0, 0, 32)->code() == TRANSPORT_TRACK_UP);
    CHECK(TileFactory::constructTile(TRANSPORT_TRACK_DOWN,  0, 0, 32)->code() == TRANSPORT_TRACK_DOWN);
}
