#ifndef NST_FRONTEND_H_INCL
#define NST_FRONTEND_H_INCL

#include "UINode.hpp"


namespace Frontend {
	void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const {

		sf::RectangleShape m_background{sf::Vector2f{10f, 6f}};

		target.draw(m_background, states);

		if (m_style.borderWidth) {
			sf::Color borderColor(m_style.bd);
			const sf::Vertex buf[]{
				{sf::Vector2f{m_position.left + 1, m_position.top + 1},                                borderColor},
				{sf::Vector2f{m_position.left + m_position.width, m_position.top + 1},                 borderColor},
				{sf::Vector2f{m_position.left + m_position.width, m_position.top + m_position.height}, borderColor},
				{sf::Vector2f{m_position.left + 1, m_position.top + m_position.height},                borderColor},
				{sf::Vector2f{m_position.left + 1, m_position.top + 1},                                borderColor}
			};
			for (int i = 0; i < 4; ++i) {
				target.draw(buf + i, 2, sf::Lines, states);
			}
		}

		for (const Container *child: m_children) {
			if (child) {
				child->draw(target, states);
			}
		}
	}

	void Container::recalculate() {
		const auto childCount{m_children.size()};
		if (m_style.flow == style::Flow::Row) {
			const auto w{m_position.width / childCount};
			sf::FloatRect size{m_position.left + 1, m_position.top + 1, w, m_position.height - 2};
			for (Container *child: m_children) {
				child->m_position = size;
				size.left += w;
				child->recalculate();
			}
		} else {
			const auto h{m_position.height / childCount};
			sf::FloatRect size{m_position.left, m_position.top, m_position.width, h};
			for (Container *child: m_children) {
				child->m_position = size;
				size.top += h;
				child->recalculate();
			}
		}
	}


}


#endif