#include <iostream>
#include <string>

#include "App.hpp"
#include "WindowExt.hpp"
#include "Image.hpp"
#include "UI.hpp"


static const UI::Style defaultStyle{
    UI::Anchor::None,
    UI::FLOW_COLUMN,
    true,
    0x101018FFu,
    0xf0f0f0FFu,
    0u,
    10u,
};

static const UI::Style defaultRowStyle{
    UI::Anchor::None,
    UI::FLOW_ROW,
    true,
    0x101018FFu,
    0xf0f0f0FFu,
    0u,
    10u,
};

App::App(const uint32_t width, const uint32_t height, const uint32_t frameRate, const std::string &title, const char *iconPath)
    : m_window(sf::VideoMode{ width, height }, title), ctx(m_window, defaultStyle) {
    m_window.setFramerateLimit(frameRate);

    if (iconPath) {
        setupIcon(iconPath);
    }
    initUI();
}


void App::setupIcon(const char *path) {
    int x, y, comp;
    uint8_t *p = stbi_load(path, &x, &y, &comp, STBI_rgb_alpha);
    if (p) {
        m_window.setIcon(x, y, p);
    }
    stbi_image_free(p);
}


void App::initUI() {
    using namespace UI;

    ctx.begin();
    {
        ctx.put<Container>(defaultStyle);
        ctx.put<Container>(defaultRowStyle)
            ->withChildren(1);
        {
            ctx.put<Container>(defaultStyle);
        }
        ctx.endChildren();
    }
    ctx.end();
}


void App::handleResize(sf::Event &event) {
    auto width{ std::max(event.size.width, MIN_WIN_WIDTH) };
    auto height{ std::max(event.size.height, MIN_WIN_HEIGHT) };
    m_window.setView(sf::View{ { 0, 0, static_cast<float>(width), static_cast<float>(height) } });
    m_window.setSize({ width, height });
}


void App::handleMousePressed(sf::Event &event) {
    switch (event.mouseButton.button) {
    case sf::Mouse::Button::Left:
    {
        // std::string filename;
        // if (!WindowExt::showFileDialog(window, filename)) {
        //     break;
        // }

        // Canvas canvas{ filename };
        // if (!canvas.isLoaded()) {
        //     break;
        // }

        // if (!m_canvases.empty()) {
        //     Canvas last{ m_canvases.back() };
        //     auto x1{ last.shape.getPosition().x + last.dimensions.x + 2 };
        //     canvas.shape.setPosition(x1, 0);
        // }
        // m_canvases.push_back(canvas);
    }
    break;

    case sf::Mouse::Button::Right:
    {

    }
    break;

    default:
        break;
    }
}

void App::handleCloseEvent(sf::Event &) {
    m_window.close();
}


void App::draw() {
    m_window.clear();

    m_window.draw(ctx);

    m_window.display();
}

int App::runUntilExit() {
    while (m_window.isOpen()) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                handleCloseEvent(event);
                break;

            case sf::Event::Resized:
                handleResize(event);
                break;

            case sf::Event::MouseButtonPressed:
                handleMousePressed(event);
                break;

            default:
                break;
            }
        }

        draw();
    }
    return EXIT_SUCCESS;
}