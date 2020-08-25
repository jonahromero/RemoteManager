// messanger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//
#include <iostream>
#include "SFML/Network.hpp"
#include "cmd.h"
#include "Options.h"
#include "Connection.h"
#include <functional>
#include <thread>
#include "ChatApp.h"
#include <initializer_list>
#include "Tokens.h"

int main(int argc, char ** argv)
{
    std::string path = argv[0];
    auto port = 50505;
    std::vector<std::string> ips = { "" };

    Options options = { { "Host","Connect", "Update IP"} };
    Connection connection;

    options[1] = [&]() {
        std::cout << "Waiting for a connection...";
        std::atomic<bool> done = false;
        std::thread t(&Connection::listen, &connection, port),
            p(&print_loading, &done);
        t.join(); done = true;
        p.join();
        std::cout << std::endl;
        ChatApp app(path, connection);
        app.start();
    };

    options[2] = [&]() {
        std::cout << "Connecting...";
        std::atomic<bool> done = false;
        void (Connection::*connect)(std::vector<std::string> const&, int) = &Connection::connect;
        std::thread t(connect, &connection, ips, port),
                    p(&print_loading, &done);
        t.join(); done = true;
        p.join();
        std::cout << std::endl;
        ChatApp app(path, connection);
        app.start();
    };

    options[3] = [&]() {
        //std::string ip, port;
        std::cout << "Please Enter the new information.";
        std::cout << std::endl << "New IP:";
        std::string new_ip;
        std::cin >> new_ip;
        ips.push_back(new_ip);
        std::cout << "New Port:";
        std::cin >> port;
        //saveIP(ip, port);
    };

    if (argc > 1 && std::string(argv[1]) == "startup") {
        system("Title system32");
        Cmd::hide_window();
        //connect to host
        options.chose(2);
    }
    else {
        system("Title Messenger");
        options.ask();
    }
}