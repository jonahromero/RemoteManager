#include "ChatApp.h"
#include <thread>
#include <string>
#include "simplified_io.h"
#include "std_messages.h"

#define ENTER_KEY 13
#define BACKSPACE 8
#define ARROW_SIGNAL 224
#define LEFT 75
#define RIGHT 77
#define CRSR ">"



ChatApp::ChatApp(std::string const& path, Connection& connection)
	: connection(connection),
	cmd(CRSR),
	clientCH([&](std::string str) { cmd.write(str); }),
	serverCH([&](std::string str) { cmd.write(str); }),
	path(path)
{
	initClientCommands(); initServerCommands(); msg_preprocess = [](std::string&) {};
}

//safely checks input, while making sure we havnt quit
int safe_get(std::atomic<bool>& flag) {
	int i;
	do {
		i = get_nb();
		if (flag) return -1;
	} while (i == -1);
	return i;
}

void ChatApp::updateMessage() {
		
	int i = safe_get(left);
	if (i == -1) return;
	bool arrow = false;
	while (i != ENTER_KEY || cmd.msg().size() == 0) {
		mutex.lock();
		if (i == ENTER_KEY) {}
		else if (i == ARROW_SIGNAL) {
			arrow = true;
		}
		else if (i == RIGHT && arrow) {
			cmd.move_right();
			arrow = false;
		}
		else if (i == LEFT && arrow) {
			cmd.move_left();
			arrow = false;
		}
		else if (i == BACKSPACE) {
			cmd.backspace();
		}
		else {
			cmd.type(i);
		}
		mutex.unlock();

		i = safe_get(left);
		if (i == -1) return;
	}
}

void ChatApp::processServerMessage(std::string msg)
{
	std::lock_guard lock(mutex);
	if (serverCH.isCommand(msg)) {
		serverCH.callCommand(msg, true);
	}
	//we make sure again, because connected might've changed by now. especially from command
	else if (!left) {
		cmd.write(msg);
	}
}

void ChatApp::processMessage() {
	//commands
	std::lock_guard lock(mutex);
	std::string msg = cmd.msg();
	if (left) return;

	if (!clientCH.isCommand(msg)) {
		msg_preprocess(msg);
	}
	if (clientCH.isCommand(msg)) {
		cmd.end_line();
		clientCH.callCommand(msg, admin);
	}
	else {
		cmd.end_line("You:");
		connection.send(username.value_or("Them") +":" + msg);
	}
}

void ChatApp::start()
{
	std::thread input_t([&]() {
		cmd.start_messaging();
		while (!left) {
			updateMessage();
			processMessage();
		}
	});

	std::thread recieve_t([&]() {
		while (!left) {
			auto [friend_msg, still_connected] = connection.recieve();
			if (!still_connected) { //if were not connected, stop recieving on thread
				cmd.write(THEY_LEFT);
				left = true;
				return;
			}
			processServerMessage(friend_msg);
		}
	});

	input_t.join();
	recieve_t.join();
	wait_key();
}
