#include "UI.hpp"
#include <stack>

namespace UI {
    Style::Style(Anchor anchor,
        const Flow flow,
        const bool border,
        const Color backgroundColor,
        const Color borderColor,
        const uint16_t xGap,
        const uint16_t yGap
    ) : anchor(anchor), flow(flow), border(border), backgroundColor(backgroundColor), borderColor(borderColor), xGap(xGap), yGap(yGap) {
    }

    Container::Container(const Container *parent, const Style &style)
        : m_parent(parent), m_children{}, m_style(style), m_background{} {
        m_background.setFillColor(style.backgroundColor);
    }

    void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        target.draw(m_background, states);

        if (m_style.border) {
            const sf::Vertex buf[]{
                {sf::Vector2f{m_position.left + 1, m_position.top + 1}, m_style.borderColor},
                {sf::Vector2f{m_position.left + m_position.width, m_position.top + 1}, m_style.borderColor},
                {sf::Vector2f{m_position.left + m_position.width, m_position.top + m_position.height}, m_style.borderColor},
                {sf::Vector2f{m_position.left + 1, m_position.top + m_position.height}, m_style.borderColor},
                {sf::Vector2f{m_position.left + 1, m_position.top + 1}, m_style.borderColor}
            };
            for (int i = 0; i < 4; ++i) {
                target.draw(buf + i, 2, sf::Lines, states);
            }
        }

        for (const Container *child : m_children) {
            if (child) {
                child->draw(target, states);
            }
        }
    }

    void Container::recalculate() {
        const auto childCount{ m_children.size() };
        if (m_style.flow == FLOW_ROW) {
            const auto w{ m_position.width / childCount };
            sf::FloatRect size{ m_position.left + 1, m_position.top + 1, w, m_position.height - 2 };
            for (Container *child : m_children) {
                child->m_position = size;
                size.left += w;
                child->recalculate();
            }
        } else {
            const auto h{ m_position.height / childCount };
            sf::FloatRect size{ m_position.left, m_position.top, m_position.width, h };
            for (Container *child : m_children) {
                child->m_position = size;
                size.top += h;
                child->recalculate();
            }
        }
    }

    void Container::setPositionAndSize(const sf::FloatRect &pos) {
        m_position = pos;
        m_background.setPosition(pos.getPosition());
        m_background.setSize(pos.getSize());
    }

    Context::~Context() {
        delete m_root;
    }



    Context::Context(const sf::RenderWindow &window, const Style &rootStyle)
        : m_root{ new Container{nullptr, rootStyle} } {
        m_currentNode = m_root;
        m_root->setPositionAndSize({
            0.,
            0.,
            static_cast<float>(window.getSize().x),
            static_cast<float>(window.getSize().y)
        });
    }

    void Context::begin() {
        if (m_root == m_currentNode) {
            m_currentNode->m_children.clear();
            return;
        };

        auto nextNode{ m_currentNode->m_children.back() };
        if (nextNode) {
            m_currentNode = nextNode;
        }
    }

    void Context::end() {
        ERROR_IF(m_currentNode && m_currentNode->m_parent,
            "Ending UI tree while not on root level");
        m_root->recalculate();
    }

    void Context::withChildren(uint32_t childCount)  {
        m_currentNode = m_currentNode->m_children.back();
        m_currentNode->m_children.resize(childCount);
    }

    void Context::endChildren() {
        ASSERT_FATAL(m_currentNode->m_parent,
                     "Trying to finish adding children without a parent element");

        m_currentNode = (Container *)(m_currentNode->m_parent);
    }


    void Context::draw(sf::RenderTarget &target, sf::RenderStates states) const {
        m_root->draw(target, states);
    }

}
