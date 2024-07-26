#ifndef SPRITE_TOOL_UINODE_HPP
#define SPRITE_TOOL_UINODE_HPP

#include "SFML/Graphics.hpp"

namespace bruh::frontend {

	class UINode : public sf::Drawable {
	protected:
		using Nodes = std::initializer_list<UINode *>;

	public:
		~UINode();

		explicit UINode(sf::Color bg, Nodes children = {});

		UINode(UINode &) = delete;

		UINode(UINode &&) = delete;

		UINode &operator=(UINode &) = delete;

		UINode &operator=(UINode &&) = delete;

		void add(Nodes children);

		// TODO: position manager
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

		void setPosition(sf::IntRect coordinates);

	private:
		std::vector<UINode *> m_children;
		sf::IntRect m_pos;
		sf::Color m_bg;
	};

	
	class Panel : public UINode {
		sf::RectangleShape m_bg;
		sf::Color m_bg_color;

	public:
		explicit Panel(sf::Color bg, Nodes children = {});
	};

}

#endif //SPRITE_TOOL_UINODE_HPP
