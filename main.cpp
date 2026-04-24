#include "Menus.h"
#include <iostream>
#include <sys/stat.h>

int main() {


    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    }
    return 0;
}
