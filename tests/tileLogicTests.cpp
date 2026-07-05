#include "doctest.h"

#include "../include/tileCode.h"
#include "../include/playerInteraction.h"

TEST_CASE("tile codes keep their expected numeric values") {
    CHECK(PLAYER_START == 0);
    CHECK(WALKABLE_GROUND == 1);
    CHECK(DESTRUCTIBLE_BLOCK == 3);
    CHECK(MIRROR_1 == 4);
    CHECK(MIRROR_2 == 5);
    CHECK(MIRROR_3 == 6);
    CHECK(MIRROR_4 == 7);
    CHECK(WATER == 8);
    CHECK(MOVABLE_BLOCK == 9);
    CHECK(FLAG == 10);
    CHECK(UNDESTRUCTABLE_BLOCK == 11);
    CHECK(ENEMY_TANK_LEFT == 12);
    CHECK(ENEMY_TANK_RIGHT == 13);
    CHECK(ENEMY_TANK_UP == 14);
    CHECK(ENEMY_TANK_DOWN == 15);
    CHECK(DESTROYED_TANK_LEFT == 16);
    CHECK(DESTROYED_TANK_UP == 17);
    CHECK(DESTROYED_TANK_DOWN == 18);
    CHECK(DESTROYED_TANK_RIGHT == 19);
    CHECK(TRANSPORT_TRACK_LEFT == 20);
    CHECK(TRANSPORT_TRACK_RIGHT == 21);
    CHECK(TRANSPORT_TRACK_UP == 22);
    CHECK(TRANSPORT_TRACK_DOWN == 23);
    CHECK(TILE_IN_WATER == 50);
}

TEST_CASE("tileIsTransportTrack only accepts the four track codes") {
    CHECK(tileIsTransportTrack(TRANSPORT_TRACK_LEFT));
    CHECK(tileIsTransportTrack(TRANSPORT_TRACK_RIGHT));
    CHECK(tileIsTransportTrack(TRANSPORT_TRACK_UP));
    CHECK(tileIsTransportTrack(TRANSPORT_TRACK_DOWN));

    CHECK_FALSE(tileIsTransportTrack(WALKABLE_GROUND));
    CHECK_FALSE(tileIsTransportTrack(WATER));
    CHECK_FALSE(tileIsTransportTrack(FLAG));
    CHECK_FALSE(tileIsTransportTrack(MOVABLE_BLOCK));
}

TEST_CASE("tileIsEnemyTank only accepts the four enemy tank codes") {
    CHECK(tileIsEnemyTank(ENEMY_TANK_LEFT));
    CHECK(tileIsEnemyTank(ENEMY_TANK_RIGHT));
    CHECK(tileIsEnemyTank(ENEMY_TANK_UP));
    CHECK(tileIsEnemyTank(ENEMY_TANK_DOWN));

    CHECK_FALSE(tileIsEnemyTank(DESTROYED_TANK_LEFT));
    CHECK_FALSE(tileIsEnemyTank(WALKABLE_GROUND));
    CHECK_FALSE(tileIsEnemyTank(MOVABLE_BLOCK));
}

TEST_CASE("player can walk onto ground, water, tracks and the flag") {
    CHECK(tileIsPlayerWalkable(WALKABLE_GROUND));
    CHECK(tileIsPlayerWalkable(WATER));
    CHECK(tileIsPlayerWalkable(TILE_IN_WATER));
    CHECK(tileIsPlayerWalkable(FLAG));
    CHECK(tileIsPlayerWalkable(TRANSPORT_TRACK_LEFT));
    CHECK(tileIsPlayerWalkable(TRANSPORT_TRACK_RIGHT));
    CHECK(tileIsPlayerWalkable(TRANSPORT_TRACK_UP));
    CHECK(tileIsPlayerWalkable(TRANSPORT_TRACK_DOWN));
}

TEST_CASE("player is blocked by solid tiles, blocks and tanks") {
    CHECK_FALSE(tileIsPlayerWalkable(MOVABLE_BLOCK));
    CHECK_FALSE(tileIsPlayerWalkable(UNDESTRUCTABLE_BLOCK));
    CHECK_FALSE(tileIsPlayerWalkable(DESTRUCTIBLE_BLOCK));
    CHECK_FALSE(tileIsPlayerWalkable(MIRROR_1));
    CHECK_FALSE(tileIsPlayerWalkable(MIRROR_2));
    CHECK_FALSE(tileIsPlayerWalkable(MIRROR_3));
    CHECK_FALSE(tileIsPlayerWalkable(MIRROR_4));
    CHECK_FALSE(tileIsPlayerWalkable(ENEMY_TANK_LEFT));
    CHECK_FALSE(tileIsPlayerWalkable(DESTROYED_TANK_LEFT));
}

TEST_CASE("a pushed block can only land on ground, water or a track") {
    CHECK(tileIsBlockPushTarget(WALKABLE_GROUND));
    CHECK(tileIsBlockPushTarget(WATER));
    CHECK(tileIsBlockPushTarget(TILE_IN_WATER));
    CHECK(tileIsBlockPushTarget(TRANSPORT_TRACK_LEFT));
    CHECK(tileIsBlockPushTarget(TRANSPORT_TRACK_RIGHT));
    CHECK(tileIsBlockPushTarget(TRANSPORT_TRACK_UP));
    CHECK(tileIsBlockPushTarget(TRANSPORT_TRACK_DOWN));

    CHECK_FALSE(tileIsBlockPushTarget(FLAG));
    CHECK_FALSE(tileIsBlockPushTarget(MOVABLE_BLOCK));
    CHECK_FALSE(tileIsBlockPushTarget(UNDESTRUCTABLE_BLOCK));
    CHECK_FALSE(tileIsBlockPushTarget(ENEMY_TANK_LEFT));
}

TEST_CASE("validMove keeps movement inside the sixteen by sixteen grid") {
    CHECK(validMove(0, 0));
    CHECK(validMove(15, 15));
    CHECK(validMove(7, 9));

    CHECK_FALSE(validMove(-1, 0));
    CHECK_FALSE(validMove(0, -1));
    CHECK_FALSE(validMove(16, 0));
    CHECK_FALSE(validMove(0, 16));
    CHECK_FALSE(validMove(16, 16));
}

TEST_CASE("tileMapPositionInBounds matches the validMove rule on every edge") {
    for (int i = -2; i < 18; i++) {
        bool inside = (i >= 0 && i < 16);
        CHECK(tileMapPositionInBounds(i, 5) == inside);
        CHECK(tileMapPositionInBounds(5, i) == inside);
    }
}
