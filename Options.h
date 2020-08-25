#pragma once
#include <vector>
#include<string>
#include <atomic>
#include <functional>

void print_loading(std::atomic<bool>* done_flag);

class Options {
	std::vector<std::string> options;
	std::vector<std::function<void()>> funcs;
public:
	Options(std::vector<std::string> const& options);
	Options(std::vector<std::string>&& options);
	std::function<void()>& operator[](int index);

	void chose(int index);
	void ask();
};