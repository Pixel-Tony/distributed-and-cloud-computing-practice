#ifndef NST_CANVAS_HPP
#define NST_CANVAS_HPP

#include "SFML/Graphics.hpp"

#include "Base.hpp"
#include <vector>
#include <string>
#include <memory>

namespace Frontend {

	class Canvas : public Container {
		sf::Image image;
		std::unique_ptr<sf::Texture> tex;
		sf::RectangleShape rect;

	public:
		bool loadFromFile(std::string &filepath);

		void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	};
}

#endif // NST_CANVAS_HPP
