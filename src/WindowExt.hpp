#ifndef _WINDOWEXT_H
#define _WINDOWEXT_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

namespace WindowExt {
    bool showFileDialog(sf::Window &, std::string &);
}

#endif