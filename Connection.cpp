#include "Connection.h"
#include <string>
#include <array>
#include <iostream>
#include "string_util.h"

bool Connection::isHost() const
{
    return wasHost;
}

void Connection::listen(int port)
{
    sf::TcpListener listener;
    if (listener.listen(port) != sf::Socket::Done) {
        throw "Was unable to listen to socket";
    }
    if (listener.accept(socket)) throw "Was unable to connect to socket";
    wasHost = true;
}

void Connection::connect(std::string const& ip, int port)
{
    while (socket.connect(ip, port) != sf::Socket::Done) {}
}

void Connection::connect(std::vector<std::string> const& ips, int port) {

    while (true) {
        for (auto& ip : ips) {
            if (socket.connect(ip, port) == sf::Socket::Done) {
                return;
            }
        }
    }
}

void Connection::disconnect()
{
    socket.disconnect();
}

void Connection::send(std::string message)
{
   /* if (message.size() > 10000) {
        std::cerr << "Messages cannot be over 10,000 characters" << std::endl;
        return;
    }*/
    if (message.size() < 1) {
        std::cerr << "Messages must be atleast 1 character" << std::endl;
        return;
    }
    sf::Packet packet;
    packet << message;
    socket.send(packet);
}

sf::IpAddress Connection::getIp() const
{
    return socket.getRemoteAddress();
}

std::pair<std::string, bool> Connection::recieve()
{
    sf::Packet packet;
    std::string recieved_data;
    auto status = socket.receive(packet);
    packet >> recieved_data;
    
    return { recieved_data , status != sf::Socket::Status::Disconnected};
}

Connection::~Connection()
{
    socket.disconnect();
}

