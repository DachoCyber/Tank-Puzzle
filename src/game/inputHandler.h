#pragma once
#include <SFML/Window/Keyboard.hpp>
#include "../../include/player.h"
#include "../../include/map.h"
#include "undoManager.h"
#include "bulletManager.h"

class PaddingGame;

class InputHandler {
public:
    InputHandler(Tank& player, Map& tileMap,
                 UndoManager& undoManager, BulletManager& bulletManager,
                 PaddingGame& padding,
                 int windowSizeX, int windowSizeY,
                 int& movesPlayed,
                 bool& returnFromTrack,
                 bool& isRepeatMovEnabled);

    void handleInput();

private:
    bool isTransportTrack(int code) const;

    Tank& player;
    Map& tileMap;
    UndoManager& undoManager;
    BulletManager& bulletManager;
    PaddingGame& padding;

    int windowSizeX, windowSizeY;
    int& movesPlayed;
    bool& returnFromTrack;
    bool& isRepeatMovEnabled;
};