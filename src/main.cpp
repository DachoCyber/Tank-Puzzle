#include "../include/game.h"
#include "../include/mainMenu/mainMenu.h"

#include "../include/mainMenu/resources.h"
#include "../include/sfmlScoreUI.h"

#include "../include/movesProcessor.h"


#include <filesystem>

namespace fs = std::filesystem;


int countMapFiles(const std::string& folderPath) {
    int count = 0;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".tmx")
                count++;
        }
    }

    return count;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

bool downloadLevel(int levelIndex) {
    if (!fs::exists("maps")) {
        fs::create_directory("maps");
    }

    std::string localFile = "maps/map" + std::to_string(levelIndex) + ".tmx";


    if (fs::exists(localFile)) {
        return true;
    }

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to init curl\n";
        return false;
    }

    std::string remoteFile = "https://alas.matf.bg.ac.rs/~mr22033/levels/uploads/map" + std::to_string(levelIndex) + ".tmx";

    FILE* fp = nullptr;
    if (fopen_s(&fp, localFile.c_str(), "wb") != 0 || !fp) {
        std::cerr << "Cannot open local file for writing: " << localFile << "\n";
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, remoteFile.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

    CURLcode res = curl_easy_perform(curl);
    fclose(fp);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK || response_code != 200) {
        
        fs::remove(localFile);
        return false;
    }
    return true;
}

void downloadAllLevels() {
    int level = 1;
    int failCount = 0;
    for(; failCount < 10; level++) {
        if (!downloadLevel(level)) {
            failCount++;
            break;
        }
	}
}

int main() {

    std::vector<LevelTable> scoreByLevels;
    curl_global_init(CURL_GLOBAL_ALL);

    downloadAllLevels();

    try {
        loadGlobalFont();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    char* folder = "maps/";
    int levelCount = countMapFiles("maps");
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
