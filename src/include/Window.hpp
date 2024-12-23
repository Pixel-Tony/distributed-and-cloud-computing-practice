#ifndef PIESHKOV_DCC_WINDOW_HPP
#define PIESHKOV_DCC_WINDOW_HPP

#include "Lab.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>


class Window {
    sf::Color m_bg_color = {0x04, 0x05, 0x0B};
    sf::RenderWindow m_rw;
    sf::Vector2u m_minSize;
    std::vector<std::shared_ptr<Lab>> m_labs;

public:
    Window(const sf::String &title, sf::Vector2u minSize, uint32_t fps, std::vector<std::shared_ptr<Lab>> labs);

    ~Window();

    void mainLoop();

    void render();
};

void setNextWindowPosSize(float rx, float ry, float rw, float rh);

#endif //PIESHKOV_DCC_WINDOW_HPP
