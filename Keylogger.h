#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "SFML/Graphics.hpp"

class Keylogger
{
private:
	using key = sf::Keyboard::Key;
	using key_map = std::unordered_map<key, std::string>;

	key_map supportedKeys;
	std::vector<key> keysPressed();
	void loadSupportedKeys();
	size_t buffer_limit;
public:
	Keylogger(size_t buffer_limit);
	//thread blocker
	void write_to(std::string& str);
};

