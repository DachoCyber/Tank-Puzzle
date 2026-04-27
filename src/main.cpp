#include "application/application.h"
#include <iostream>

int main() {
    try {
        Application app;
        return app.run();
    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << '\n';
        return -1;
    }
}