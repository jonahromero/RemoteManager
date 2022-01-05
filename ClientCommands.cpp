#include "ChatApp.h"
#include "std_messages.h"
#include "command_guide.h"
#include <fstream>
#include <sstream>
#define FILE_PACKET_SIZE 600
#define ADMIN_PASS "123"

void ChatApp::initClientCommands()
{
	using cm = CommandHandler;
	//NORMAL COMMANDS
	clientCH.addCommand("/leave", [&](cm::Params args) {
		//tell other guy
		connection.send("/leave");
		connection.disconnect();
		cmd.write(YOU_LEFT);
		left = true;
	});

	clientCH.addCommand("/name", [&](cm::Params args) {
		if (args[0].size() > 0) username = args[0];
	});

	clientCH.addCommand("/clear", [&](cm::Params args) {
		system("CLS");
	});

	clientCH.addCommand("/admin", [&](cm::Params args) {
		if (args[0] == ADMIN_PASS) {
			admin = true;
		}
	});

	clientCH.addCommand("/help", [=](cm::Params args) {
		std::string result = "Avaialable Commands:\n";
		for (int i = 0; i < commands.size(); i++) {
			result.append(commands[i]);
			result.push_back('\n');
		}
		if (admin) {
			for (int i = 0; i < admin_commands.size(); i++) {
				result.append(admin_commands[i]);
				result.push_back('\n');
			}
		}
		result.pop_back();
		cmd.write(result);
	});

	//ADMIN COMMANDS
	clientCH.addCommand("/shelp", [&](cm::Params args) {
		std::string result = "Avaialable Server Commands:\n";
		for (int i = 0; i < server_commands.size(); i++) {
			result.append(server_commands[i]);
			result.push_back('\n');
		}
		result.pop_back();
		cmd.write(result);
	}, true);

	clientCH.addCommand("/bmode", [&](cm::Params args) {
		msg_preprocess = [](std::string& str) {
			str.append("]]");
			str.insert(0, "/s [/batch [");
		};
	}, true);

	clientCH.addCommand("/smode", [&](cm::Params args) {
		msg_preprocess = [](std::string& str) {
			str.push_back(']');
			str.insert(0, "/s [");
		};
	}, true);

	clientCH.addCommand("/dmode", [&](cm::Params args) {
		msg_preprocess = [](std::string& str) {};
	}, true);

	clientCH.addCommand("/s", [&](cm::Params args) {
		connection.send(args[0]);
	}, true);

	clientCH.addCommand("/download", [&](cm::Params args) {
		connection.send("/c [/send_file [" + args[1] + "] [" + args[0] + "]]");
	}, true);

	clientCH.addCommand("/send_file", [&](cm::Params p_args) {

		std::thread send_file([&](cm::Params args) {
			
			std::ifstream file(args[0]);
			if (file.is_open()) {
				std::stringstream stream;
				stream << file.rdbuf();
				auto contents = stream.str();
				if (contents.size() == 0) return;
				
				int loads = contents.size() / FILE_PACKET_SIZE,
					extra = contents.size() % FILE_PACKET_SIZE;
				std::lock_guard lock(mutex);
				int last_index = 0;
				for (int i = 0; i < loads; i++) {
					std::string packet = contents.substr(last_index, FILE_PACKET_SIZE);
					last_index += FILE_PACKET_SIZE;
					auto pack_size = packet.size();
					connection.send("/save [" + args[1] + "] " + std::to_string(pack_size) + "[" + packet + "]");
				}
				if (extra > 0) {
					std::string extra_content = contents.substr(last_index, extra);
					auto extra_size = extra_content.size();
					connection.send("/save [" + args[1] + "] " + std::to_string(extra_size) + "[" + extra_content + "]");
				}
			}
			else {
				std::lock_guard lock(mutex);
				cmd.write("Unable to open file at: " + args[0]);
			}
		}, std::move(p_args));

		send_file.detach();
	}, true);

	clientCH.addCommand("/ip", [&](cm::Params args) {
		cmd.write(connection.getIp().toString());
	}, true);
}
