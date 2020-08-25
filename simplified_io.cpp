#include "simplified_io.h"
#include "string_util.h"
#include <iostream>
#include <conio.h>
#include <thread>
#include <array>

void print(std::string const& message) {
	std::cout << message;
}

char get() {
	return _getch();
}

int get_nb() {
	if (_kbhit())
		return _getch();
	else
		return -1;
}

void wait_key()
{
	_getch();
}

int getNumber() {
	std::string input_s;
	std::getline(std::cin, input_s);
	if (isNumber(input_s)) {
		return std::stoi(input_s);
	}
	return -1;
}

void print_loading(std::atomic<bool>* done_flag)
{
	static constexpr std::array<char, 4> symbols = { '/','-','\\','|' };
	std::cout << symbols[0];
	while (!done_flag->load()) {
		for (auto const& symbol : symbols) {
			std::cout << '\b' << symbol;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}