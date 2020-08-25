#pragma once
#include <string>
#include <functional>

class CommandHandler
{
public:
	using Params = std::vector<std::string>&&;
	using Command = std::function<void(Params)>;
	using ErrorHandler = std::function<void(std::string)>;
	CommandHandler(ErrorHandler errorHandler);

	void addCommand(std::string_view name, Command command, bool admin = false);
	void callCommand(std::string const& command, bool admin);
	bool isCommand(std::string const& command);
	
private:
	static void formatArg(std::string& str);
	void formatArgs(std::vector<std::string>& args);
	std::unordered_map<std::string, std::pair<Command, bool>> commands;
	ErrorHandler errorHandler;
};
