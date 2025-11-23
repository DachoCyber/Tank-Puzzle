#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

std::string extractStringValue(const std::string& line);
int extractIntValue(const std::string& line);

struct PlayerTable {
    std::string name;
    int score;

    bool operator==(const PlayerTable& other) const {
        return name == other.name;
    }
};

struct LevelTable {
    std::vector<PlayerTable> players;
};

std::vector<PlayerTable> getPlayerScores(int targetLevel);
