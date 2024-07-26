#include "Canvas.hpp"

void Canvas::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(rect, states);
}

bool Canvas::loadFromFile(std::string &filepath) {
	if (!image.loadFromFile(filepath))
		return false;

	tex = std::make_unique<sf::Texture>();
    if (!tex->loadFromImage(image)) {
		tex.reset();
		return false;
	}

	auto [w, h] = image.getSize();
	rect = sf::RectangleShape(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
    rect.setTexture(tex.get());

	return true;
}