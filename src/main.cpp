#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

#include "SFML/Graphics.hpp"

#include "debug.hpp"

#define MIN_WIN_WIDTH 200u
#define MIN_WIN_HEIGHT 150u
#include "App.hpp"

#include "UI.hpp"

#if !(defined(FILE_ICON_PNG) && defined(FILE_ICON))
#error "both FILE_ICON_PNG & FILE_ICON must be defined"
#endif

#ifdef WIN32
#define ENTRANCE_POINT_NAME WinMain
#else
#define ENTRANCE_POINT_NAME main
#endif

int main() {
    App app(1280u, 720u, 60u, "Hello there", FILE_ICON_PNG);
    return app.runUntilExit();
}
