#pragma once
#include "SFML/Network.hpp"
#include <string>

class Connection
{
	sf::TcpSocket socket;
	bool wasHost = false;
public:
	bool isHost() const;
	void listen(int port);
	void connect(std::string const& ip, int port);
	void connect(std::vector<std::string> const& ips, int port);
	void disconnect();
	void send(std::string message);
	sf::IpAddress getIp() const;
	//message, and if they're still connected
	std::pair<std::string, bool> recieve();

	~Connection();
};

