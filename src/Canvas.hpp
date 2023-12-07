#ifndef _CANVAS_H
#define _CANVAS_H

#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <memory>

struct Canvas : public sf::Drawable {
    std::vector<uint32_t> ptr;
    sf::Vector2i dimensions;
    int channels;
    std::shared_ptr<sf::Texture> texture;
    sf::RectangleShape shape;
    bool loaded;

public:
    Canvas(std::string &filepath);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    inline constexpr
    bool isLoaded() const {
        return loaded;
    }

    static constexpr
    int bytesPerPixel = 4;

};

#endif
