#include "../tinyxml2/tinyxml2.h"
#include "../include/extractMatrix.h"
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace tinyxml2;

/**
 * We assume map is of the CSV format,
 * not .tmx .
 */

const int N = 16;
void readMapFromXML(std::vector<std::vector<int>>& map, int level) {
    map.resize(N, std::vector<int>(N));

    const char* lvlPath = strcat(strcat("maps/map", std::to_string(level).c_str()), ".txt");
    std::ifstream file(lvlPath);
    
    if (!file.is_open()) {
        std::cerr << "Can not open file " << lvlPath << std::endl;
        return;
    }

    std::istringstream stream;
    std::string line;
    int currCode = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string comma;
        int code;
        

        if (currCode < 255) {
            if (iss >> code >> comma) {
                std::cout << "File is not of a good format!" << std::endl;
            }
        }
        else {
            if (iss >> code) {
                std::cout << "File is not of a good format!" << std::endl;
            }
        }

        map[currCode / 16][currCode % 16] = code;

        currCode++;
    }
}
