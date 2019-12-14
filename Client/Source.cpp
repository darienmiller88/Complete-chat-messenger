#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <future>

using namespace sf;

void sendMessage(sf::TcpSocket &serverSocket, const std::string &name) {
	std::string message, client = "Client: ";
	sf::Packet sendPacket;
	sf::Socket::Status status;

	do{
		//std::cout << "Enter message >";
		std::getline(std::cin, message);

		sendPacket << name + ": " + message;
		status = serverSocket.send(sendPacket);
		sendPacket.clear();
	} while (status != sf::Socket::Disconnected);

}

void receiveMessage(sf::TcpSocket& serverSocket) {
	std::string message, joinedChat;
	sf::Packet receivePacket, chatPacket;
	sf::Socket::Status status;

	do{
		//In addition sending a message in a Packet, we will also be receiving a message from the Server!
		status = serverSocket.receive(receivePacket);

		//Error handle to make sure that we aren't extracting more data from the packet than what was originally there
		if (receivePacket >> message) {
			std::cout << "\n" << message << "\n";
		}

		/*if (serverSocket.receive(chatPacket) == sf::Socket::Done and chatPacket >> joinedChat)
			std::cout << joinedChat << "\n";*/

	} while (status != sf::Socket::Disconnected);
	
}

int main() {
	sf::TcpSocket serverSocket;
	auto status = serverSocket.connect(sf::IpAddress::LocalHost, 2000);
	std::string name, joinedChat;
	sf::Packet packet, receivePacket;

	if (status != sf::Socket::Done) {
		std::cout << "Could not connect to server :(\n";
		system("pause");
		exit(1);
	}

	std::cout << "Joined server! Enter your id! ";
	std::getline(std::cin, name);

	packet << name;
	serverSocket.send(packet);
	
	auto future = std::async(std::launch::async, sendMessage, std::ref(serverSocket), name);
	receiveMessage(serverSocket);
}