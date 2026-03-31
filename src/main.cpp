#include "../include/game.h"
#include "../include/mainMenu/mainMenu.h"

#include "../include/mainMenu/resources.h"
#include "../include/sfmlScoreUI.h"

#include "../include/movesProcessor.h"


#include <filesystem>

int countMapFiles(char* fileName) {
    FILE* fptr = fopen(fileName, "r");
    if(fptr == NULL)
        exit(1);
    int file_num;
    fscanf(fptr, "%d", &file_num);
    fclose(fptr);
    return file_num;
}

int main() {

    std::vector<LevelTable> scoreByLevels;
    curl_global_init(CURL_GLOBAL_ALL);
    

	std::string url = "https://alas.matf.bg.ac.rs/~mr22033/~public_html/levels/";


    try {
        loadGlobalFont();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    char* folder = "maps/";
    int levelCount = countMapFiles("src/levelCount.txt");
    scoreByLevels.resize(levelCount + 1);

    bool getIsClosed = false;
    MainMenu menu(levelCount);
    menu.run();

    bool backClicked = false;
    bool enterAnotherLevel = false;
    int chosenLevel;
    bool menuWindowClose = false;
    do {
        
        if (!enterAnotherLevel) {
            chosenLevel = menu.getChosenLevel();
        }
        if(backClicked) {
            MainMenu menu(levelCount);
            menu.run();
            menuWindowClose = menu.getWinClose();
            chosenLevel = menu.getChosenLevel();
            menu.setChosenLevel(chosenLevel);
        }
        if (chosenLevel != -1) {
            MainGame game(700, 512, 512, 512, chosenLevel);
            game.run();
            backClicked = game.isBackClicked();
            getIsClosed = !game.getWindowClosedState();
            if (game.gameWon()) {
                
                enterAnotherLevel = true;
        
                std::string initials = game.getInitials();
                curl_global_init(CURL_GLOBAL_ALL);
                std::vector<PlayerTable> players = getPlayerScores(chosenLevel);
                scoreByLevels[chosenLevel].players = players;

                sendScore(initials, game.getMovesCount(), chosenLevel);
                sfmlUi(initials, chosenLevel, game.getMovesCount());
                curl_global_cleanup();


                chosenLevel = (chosenLevel + 1) % levelCount;
	    }
	  
        }
    } while (getIsClosed && !menuWindowClose);
    curl_global_cleanup();
    return 0;
}
