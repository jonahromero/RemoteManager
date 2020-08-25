#include "CommandHandler.h"
#include "std_messages.h"
#include <cctype>
#include <optional>
#include "string_util.h"


CommandHandler::CommandHandler(ErrorHandler errorHandler) : errorHandler(errorHandler) {}

void CommandHandler::addCommand(std::string_view name, Command command, bool admin)
{
	commands.emplace(name, std::pair{command, admin});
}

void CommandHandler::callCommand(std::string const& command, bool admin)
{
	auto split_command = bracket_split(command);
	auto it = commands.find(split_command[0]);

	if (it != commands.end()) {
		split_command.erase(split_command.begin()); //remove first arg
		
		if (admin || !it->second.second) {
			formatArgs(split_command);
			it->second.first(std::move(split_command));
		}
		else {
			errorHandler(INVALID_PERMS);
		}
	}
	else {
		errorHandler(INVALID_PERMS);
	}
}

bool CommandHandler::isCommand(std::string const& command)
{
	if (command.size() == 0) return false;
	auto splited = bracket_split(command);
	auto first_token = splited[0];
	return commands.find(first_token) != commands.end();
}

void CommandHandler::formatArgs(std::vector<std::string>& args)
{
	for (auto& arg : args) {
		CommandHandler::formatArg(arg);
	}
}

void CommandHandler::formatArg(std::string& arg)
{
	size_t pos = arg.find('[');
	if (pos != std::string::npos) {
		if (pos == 0 && arg.back() == ']') {
			arg = arg.substr(1, arg.size() - 2);
		}
		else if (isNumber(arg.substr(0, pos - 1))) {
			arg = arg.substr(pos + 1, arg.size() - 2);
		}
	}
}


