#include "fileUtils.h"
#include <filesystem>

namespace fs = std::filesystem;

int countMapFiles(const std::string& folderPath) {
    int count = 0;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) continue;
        std::string name = entry.path().filename().string();
        if (name.size() > 4 && name.substr(name.size() - 4) == ".tmx")
            count++;
    }
    return count;
}