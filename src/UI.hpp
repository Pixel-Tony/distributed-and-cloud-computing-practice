#ifndef _UI_H
#define _UI_H

#include <vector>
#include <concepts>
#include <memory>

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "debug.hpp"


namespace UI {
    class Context;

    using Color = sf::Color;

    typedef bool Flow;
    const Flow FLOW_ROW{ false };
    const Flow FLOW_COLUMN{ true };

    enum class Anchor : char {
        None, N, NW, W, SW, S, SE, E, NE
    };

    struct Style {
        Anchor anchor;
        Flow flow;
        bool border;
        Color backgroundColor;
        Color borderColor;
        uint16_t xGap;
        uint16_t yGap;

        Style(Anchor anchor = Anchor::None,
            const Flow flow = FLOW_ROW,
            const bool border = false,
            const Color backgroundColor = Color::Black,
            const Color borderColor = Color::Transparent,
            const uint16_t xGap = 0u,
            const uint16_t yGap = 0u
        );
    };

    class Container : public sf::Drawable {
        friend class Context;

        const Container *m_parent;
        std::vector<Container *> m_children;

        Container(Container &&a) = delete;
        Container(Container &a) = delete;
        Container() = delete;
        ~Container() {
            for (Container *a : m_children) {
                delete a;
            }
        }

        Style m_style;
        sf::FloatRect m_position;
        sf::RectangleShape m_background;

        Container(const Container *parent, const Style & = Style{});

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

        void recalculate();

        void setPositionAndSize(const sf::FloatRect &);
    };

    template<typename T>
    concept ContainerLike = std::is_base_of_v<Container, T>;

    class Context : public sf::Drawable {
        Container *m_currentNode;
        Container *m_root;

    public:
        ~Context();
        Context(Context &copy) = delete;
        Context(const sf::RenderWindow &, const Style &rootStyle);

        void begin();
        void end();
        void withChildren(uint32_t childCount);
        void endChildren();

        template <ContainerLike T>
        constexpr Context *put(const Style &style) {
            ASSERT_FATAL(m_currentNode,
                         "Cannot put child container: parent container does not exist");
            m_currentNode->m_children.push_back(new T{ m_currentNode, style });
            return this;
        }

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    };

}


#endif