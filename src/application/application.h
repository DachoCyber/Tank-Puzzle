#pragma once
#include <vector>
#include <string>
#include "../../include/movesProcessor.h"
#include "../../include/getScores.h"

class Application {
public:
    Application();
    int run();

private:
    void handleLevelWon(int chosenLevel, const std::string& initials, int moves);

    std::vector<LevelTable> scoreByLevels;
    int levelCount = 0;
};