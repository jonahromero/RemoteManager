#pragma once
#include <string>
#include <optional>

class Cmd {
	std::string const cursor;
	std::string current_msg;
	int current_index{};
	std::optional<std::string> dir;
public:
	Cmd(std::string_view cursor);
	void start_messaging();
	//io
	void backspace();
	void type(char letter);
	void move_left();
	void move_right();
	void end_line(std::string const& prefix = "");
	void write(std::string const& msg);

	//getter
	std::string const& msg() const;

	//functionality
	void set_directory(std::string const&);
	std::string pipe_call(std::string const& cmd) const;
	static void hide_window();
};