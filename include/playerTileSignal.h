// ################################
// 
// enum class for tile interaction
//       with player (tank)
//
// ###############################
#pragma once

enum class PlayerTileSignal {
    
    // move player (conveyer)
    MOVE_PLAYER_RIGHT,
    MOVE_PLAYER_LEFT,
    MOVE_PLAYER_DOWN,
    MOVE_PLAYER_UP,

    // kill player
    KILL_PLAYER,

    // block player from entering, like movable block
    BLOCK_PLAYER,

    // no signal (walkable ground, tileInWater)
    NONE
};