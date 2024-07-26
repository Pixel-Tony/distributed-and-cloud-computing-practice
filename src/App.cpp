#include "App.hpp"
#include "Frontend/WindowExt.hpp"



App::App(uint32_t width, uint32_t height, uint32_t frameRate, const std::string &title)
	: m_window(sf::VideoMode{width, height}, title) {
	m_window.setFramerateLimit(frameRate);
	initUI();
}

void App::setupIcon(std::string &filePath) {
	sf::Image image;
	if (!image.loadFromFile(filePath))
		return;

	auto [w, h] = image.getSize();
	m_window.setIcon(w, h, image.getPixelsPtr());
}


void App::initUI() {
	//
}


void App::handleResize(sf::Event &event) {
	auto width{std::max(event.size.width, MIN_WIN_WIDTH)};
	auto height{std::max(event.size.height, MIN_WIN_HEIGHT)};
	m_window.setView(sf::View{{0, 0, static_cast<float>(width), static_cast<float>(height)}});
	m_window.setSize({width, height});
}


void App::handleMousePressed(sf::Event &event) {
	switch (event.mouseButton.button) {
		case sf::Mouse::Button::Left: {
			std::string filename;
			if (!WindowExt::showFileDialog(m_window, filename)) {
				break;
			}

			Canvas canvas;
			if (!canvas.loadFromFile(filename)) {
				break;
			}

			if (!m_canvases.empty()) {
				Canvas last{m_canvases.back()};
				auto xlast{last.rect.getPosition().x + last.image.getSize().x};
				canvas.rect.setPosition(xlast + 2, 0);
			}
			m_canvases.push_back(canvas);
		}
			break;

		case sf::Mouse::Button::Right: {

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
	m_window.display();
}

int App::runUntilExit() {
	while (m_window.isOpen()) {
		for (sf::Event event; m_window.pollEvent(event);) {

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