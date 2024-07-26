#include "UINode.hpp"

namespace bruh::frontend {
	UINode::~UINode() {
		for (auto &child: m_children)
			delete child;
	}

	UINode::UINode(sf::Color bg, Nodes children)
		: m_children(children), m_bg(bg) {}

	void UINode::add(Nodes children) {
		if (m_children.empty())
			m_children = children;
		else
			m_children.insert(m_children.end(), children);
	}

	void UINode::setPosition(sf::IntRect coordinates) {
		m_pos = coordinates;
	}

}

