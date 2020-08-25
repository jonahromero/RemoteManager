#pragma once
#include "Connection.h"
#include "CommandHandler.h"
#include <mutex>
#include <atomic>
#include <optional>
#include "cmd.h"

class ChatApp
{
	Cmd cmd;
	Connection& connection;
	//command calls gaurenteed to be locked
	CommandHandler clientCH, serverCH;
	std::optional<std::string> username;
	std::atomic<bool> left = false;
	bool admin = false;
	std::function<void(std::string&)> msg_preprocess;
	std::mutex mutex;
	std::string path;

	void processServerMessage(std::string msg);
	void processMessage();
	void updateMessage();
public:
	ChatApp(std::string const& path, Connection& connection);
	void start();
	void initClientCommands();
	void initServerCommands();
};

