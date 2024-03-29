#pragma once
#include <SFML/Network.hpp>
#include <unordered_map>

/*Bare Bones C++ server using SFML networking utilities*/
class Server{
	public:
		Server(uint16_t portToListen);
		void runServer();
	private:
		void listenForClients();
		void processClients();
		void notifyClientsOfNewClient();
	private:
		bool clientJoined;
		sf::TcpListener listener;
		sf::SocketSelector selector;
		std::string clientJoinedMessage, nameOfNewClient;
		std::unordered_map< std::string, std::unique_ptr<sf::TcpSocket> > clients;
};