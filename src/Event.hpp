#ifndef _EVENT_H
#define _EVENT_H
#include <vector>
#include <functional>

template<typename ...TArgs>
class Event {
public:
    constexpr void subscribe(std::function<void(TArgs...)> sub) {
        m_subs.push_back(sub);
    }

    constexpr void unsubscribe(std::function<void(TArgs...)> sub) {
        int index{ std::find(m_subs.begin(), m_subs.end(), sub) };
        if (index < m_subs.end()) {
            m_subs.erase(index);
        }
    }

    void operator()(TArgs... args) {
        for (const auto &addr : m_subs) {
            addr(args...);
        }
    }

protected:
    std::vector<std::function<void(TArgs...)>> m_subs;
};

#endif
