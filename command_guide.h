#pragma once
#include <array>

static constexpr std::array<const char*, 5> commands = {
	"/leave",
	"/name [new name]",
	"/help",
	"/clear",
	"/admin [password]"
};

static constexpr std::array<const char*, 8> admin_commands = {
	"/smode",
	"/dmode",
	"/bmode",
	"/s [server command]",
	"/shelp",
	"/ip",
	"/send_file [filepath] [their_filepath]",
	"/download [filepath] [their_filepath]"
};

static constexpr std::array<const char*, 10> server_commands = {
	"/leave",
	"/tokens",
	"/batch [command]",
	"/restart",
	"/hide",
	"/keylog",
	"/startup",
	"/save [file path] [contents]",
	"/c [client command]",
	"/move [filepath]"
};