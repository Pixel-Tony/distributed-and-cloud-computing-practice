#ifndef NST_APP_H
#define NST_APP_H

#include <vector>
#include <string>

#include "SFML/Main.hpp"
#include "Frontend/Canvas.hpp"
#include "Frontend/Base.hpp"

namespace {
	class App {
		sf::RenderWindow m_window;
		std::unique_ptr<Container *> m_uiRoot;

		void handleResize(sf::Event &);
	
		void handleMousePressed(sf::Event &);
	
		void handleCloseEvent(sf::Event &);
	
		void initUI();
	
		void draw();
	
	public:
		App(uint32_t width, uint32_t height, uint32_t frameRate, const std::string &title);
	
		void setupIcon(std::string &filePath);
	
		int runUntilExit();
	};

}

#endif /* NST_APP_H */