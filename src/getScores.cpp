#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

#include "../include/getScores.h"

bool noInternetConnection = false;


size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, totalSize);
    return totalSize;
}

std::string extractStringValue(const std::string& line) {
    int colon = line.find(":");
    int q1 = line.find("\"", colon + 1);
    int q2 = line.find("\"", q1 + 1);
    return line.substr(q1 + 1, q2 - q1 - 1);
}

int extractIntValue(const std::string& line) {
    int colon = line.find(":");
    std::string num = line.substr(colon + 1);
    return std::stoi(num);
}




std::vector<PlayerTable> getPlayerScores(int targetLevel) {
    noInternetConnection = false;

    int maximum = 100;

    std::vector<PlayerTable> players;

    std::string url = "https://alas.matf.bg.ac.rs/~mr22033/data.json";

    CURL* curl = curl_easy_init();
    std::string json;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: "
                      << curl_easy_strerror(res) << "\n";
            noInternetConnection = true;
            return {};
        }
        curl_easy_cleanup(curl);
    }

    std::stringstream ss(json);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    int k = 0;
    int i = 0;
    while (i < maximum) {
        i++;
        int nameIndex  = 2 + 6 * k;
        int scoreIndex = 3 + 6 * k;
        int levelIndex = 4 + 6 * k;

        if (levelIndex >= (int)lines.size())
            break;

        int level = extractIntValue(lines[levelIndex]);

        if (level == targetLevel) {
            std::string name  = extractStringValue(lines[nameIndex]);
            int score = extractIntValue(lines[scoreIndex]);

            std::cout << "Match found:\n";
            std::cout << "Name: " << name << "\n";
            std::cout << "Score: " << score << "\n";

            PlayerTable player;
            player.score = score;
            player.name = name;

            players.push_back(player);
        }

        k++;
    }

    return players;
}