#ifndef _APP_H
#define _APP_H

#include <vector>

#include "SFML/Main.hpp"
#include "Canvas.hpp"
#include "UI.hpp"

#ifndef MIN_WIN_WIDTH
#define MIN_WIN_WIDTH 0u
#endif
#ifndef MIN_WIN_HEIGHT
#define MIN_WIN_HEIGHT 0u
#endif

class App {
    sf::RenderWindow m_window;
    std::vector<Canvas> m_canvases;
    UI::Context ctx;

    void setupIcon(const char *filePath);

    void handleResize(sf::Event &);
    void handleMousePressed(sf::Event &);
    void handleCloseEvent(sf::Event &);

    void initUI();

    void draw();

public:
    App(const uint32_t width, const uint32_t height, const uint32_t frameRate, const std::string &title, const char *iconPath);
    int runUntilExit();
};

#endif /* _APP_H */