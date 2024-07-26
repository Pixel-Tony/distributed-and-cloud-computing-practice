#ifndef SPRITE_TOOL_PARSING_HPP
#define SPRITE_TOOL_PARSING_HPP

#include "SFML/Graphics.hpp"
#include <concepts>

bool fromValue(uint32_t rgba, int size, sf::Color &dest) {
	switch (size) {
		case 3:
			dest = sf::Color(17 * (rgba >> 16), 17 * (rgba >> 8), 17 * rgba, 255);
			return true;
		case 4:
			dest = sf::Color(17 * (rgba >> 24), 17 * (rgba >> 16), 17 * (rgba >> 8), 17 * rgba);
			return true;
		case 6:
			dest = sf::Color(rgba >> 16, rgba >> 8, rgba, 255);
			return true;
		case 8:
			dest = sf::Color(rgba >> 24, rgba >> 16, rgba >> 8, rgba);
			return true;
		default:
			return false;
	}
}

template<std::integral T>
bool parseNum(T &destination, auto &src) {
	T res = 0;

	for (auto n: src) {
		n -= '0';
		if (n < 0 || n > 9)
			return false;
		auto nRes{10 * res + n};

		// overflow reached
		if (nRes < res)
			return false;

		res = nRes;
	}

	destination = res;
	return true;
}

template<std::integral T>
bool parseHex(T &destination, auto &src) {
	T res = 0;

	for (auto n: src) {
		if (n >= '0' && n <= '9')
			n -= '0';
		else if (n >= 'a' && n <= 'f')
			n -= 'a' - 10;
		else if (n >= 'A' && n <= 'F')
			n -= 'A' - 10;
		else return false;

		auto nRes{16 * res + n};

		// overflow reached
		if (nRes < res)
			return false;

		res = nRes;
	}

	destination = res;
	return true;
}

#endif // SPRITE_TOOL_PARSING_HPP