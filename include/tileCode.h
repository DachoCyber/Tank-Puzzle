#pragma once

enum TileCode {
    PLAYER_START          = 0,
    WALKABLE_GROUND       = 1,
    DESTRUCTIBLE_BLOCK    = 3,
    MIRROR_1              = 4,
    MIRROR_2              = 5,
    MIRROR_3              = 6,
    MIRROR_4              = 7,
    WATER                 = 8,
    MOVABLE_BLOCK         = 9,
    FLAG                  = 10,
    UNDESTRUCTABLE_BLOCK  = 11,
    ENEMY_TANK_LEFT       = 12,
    ENEMY_TANK_RIGHT      = 13,
    ENEMY_TANK_UP         = 14,
    ENEMY_TANK_DOWN       = 15,
    DESTROYED_TANK_LEFT   = 16,
    DESTROYED_TANK_UP     = 17,
    DESTROYED_TANK_DOWN   = 18,
    DESTROYED_TANK_RIGHT  = 19,
    TRANSPORT_TRACK_LEFT  = 20,
    TRANSPORT_TRACK_RIGHT = 21,
    TRANSPORT_TRACK_UP    = 22,
    TRANSPORT_TRACK_DOWN  = 23,
    TILE_IN_WATER         = 50
};

inline bool tileIsTransportTrack(int code) {
    return code == TRANSPORT_TRACK_LEFT  || code == TRANSPORT_TRACK_RIGHT ||
           code == TRANSPORT_TRACK_UP    || code == TRANSPORT_TRACK_DOWN;
}

inline bool tileIsEnemyTank(int code) {
    return code == ENEMY_TANK_LEFT || code == ENEMY_TANK_RIGHT ||
           code == ENEMY_TANK_UP   || code == ENEMY_TANK_DOWN;
}

inline bool tileIsPlayerWalkable(int code) {
    return code == WALKABLE_GROUND || code == WATER || code == TILE_IN_WATER ||
           code == FLAG || tileIsTransportTrack(code);
}

inline bool tileIsBlockPushTarget(int code) {
    return code == WALKABLE_GROUND || code == WATER || code == TILE_IN_WATER ||
           tileIsTransportTrack(code);
}

inline bool tileMapPositionInBounds(int x, int y) {
    return x >= 0 && x < 16 && y >= 0 && y < 16;
}
