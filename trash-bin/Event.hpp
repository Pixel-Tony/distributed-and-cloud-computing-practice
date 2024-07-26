#ifndef NST_EVENT_HPP
#define NST_EVENT_HPP

#include <vector>
#include <functional>


template<typename ...TArgs>
class Event {
protected:
	std::vector<sub_t> m_subs;

public:
	using sub_t = std::function<void(TArgs...)>;

	void subscribe(sub_t sub) {
		m_subs.push_back(sub);
	}

	void unsubscribe(sub_t sub) {
		int index{std::find(m_subs.begin(), m_subs.end(), sub)};
		if (index < m_subs.end())
			m_subs.erase(index);
	}

	Event &operator+(sub_t sub) {
		subscribe(sub);
		return this;
	}

	Event &operator-(sub_t sub) {
		unsubscribe(sub);
		return this;
	}

	void operator()(TArgs... args) {
		for (auto &sub: m_subs)
			sub(args...);
	}
};

#endif // NST_EVENT_HPP