#include "application.h"
#include "../network/LevelDownloader.h"
#include "../utils/FileUtils.h"
#include "../../include/mainMenu/mainMenu.h"
#include "../../include/mainMenu/resources.h"
#include "../../include/sfmlScoreUI.h"
#include "../../include/movesProcessor.h"
#include "../game/mainGame.h"

#include <curl/curl.h>
#include <iostream>

Application::Application() {
    curl_global_init(CURL_GLOBAL_ALL);
    downloadAllLevels();

    loadGlobalFont();

    levelCount = countMapFiles("maps");
    scoreByLevels.resize(levelCount + 1);
}

void Application::handleLevelWon(int chosenLevel,
                                  const std::string& initials, int moves) {

    std::vector<PlayerTable> players = getPlayerScores(chosenLevel);
    scoreByLevels[chosenLevel].players = players;
    sendScore(initials, moves, chosenLevel);
    sfmlUi(initials, chosenLevel, moves);
}

int Application::run() {
    bool appRunning   = true;
    bool menuClosed   = false;
    bool backClicked  = false;
    bool wonLastLevel = false;
    int  chosenLevel  = -1;

    {
        MainMenu menu(levelCount);
        menu.run();
        menuClosed  = menu.getWinClose();
        chosenLevel = menu.getChosenLevel();

        if (menuClosed || chosenLevel == -1) {
            curl_global_cleanup();
            return 0;
        }
    }

    do {

        if (backClicked) {
            MainMenu menu(levelCount);
            menu.run();
            menuClosed = menu.getWinClose();
            chosenLevel = menu.getChosenLevel();
            backClicked = false;
        }

        if (chosenLevel == -1) {
            return 0;
        }
        MainGame game(700, 512, 512, 512, chosenLevel);
        game.run();


        backClicked = game.isBackClicked();
        appRunning  = !game.getWindowClosedState();

        if (game.gameWon()) {
            wonLastLevel = true;
            handleLevelWon(chosenLevel, game.getInitials(), game.getMovesCount());
            chosenLevel = (chosenLevel + 1) % levelCount;
        } else {
            wonLastLevel = false;
        }
    } while (appRunning && !menuClosed);

    curl_global_cleanup();
    return 0;
}