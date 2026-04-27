#include "levelDownloader.h"
#include <curl/curl.h>
#include <filesystem>
#include <iostream>
#include <cstdio>

namespace fs = std::filesystem;

static size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

bool downloadLevel(int levelIndex) {
    if (!fs::exists("maps"))
        fs::create_directory("maps");

    std::string localFile = "maps/map" + std::to_string(levelIndex) + ".tmx";
    if (fs::exists(localFile))
        return true;

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to init curl\n";
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 9L);
    CURLcode resConnection = curl_easy_perform(curl);

    if (resConnection == CURLE_OPERATION_TIMEOUTED) {
        std::cout << "Can not download levels!\n" << std::flush;
        return false;
    }

    std::string remoteFile =
        "https://alas.matf.bg.ac.rs/~mr22033/levels/uploads/map"
        + std::to_string(levelIndex) + ".tmx";

    FILE* fp = nullptr;
    if (fopen_s(&fp, localFile.c_str(), "wb") != 0 || !fp) {
        std::cerr << "Cannot open local file for writing: " << localFile << "\n";
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL,           remoteFile.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,     fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT,       30L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE,       0L);

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
    for (int level = 1; ; level++) {
        if (!downloadLevel(level))
            break;
    }
}