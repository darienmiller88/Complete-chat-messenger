#include "Server.h"
#include <iostream>
#include <future>

Server::Server(uint16_t portToListen) : clientJoined(false) {
	// bind the listener to a port	
	if (listener.listen(portToListen) != sf::Socket::Done) {
		std::cerr << "Could not listen to port: " << listener.getLocalPort() << " for whatever reason :/";
		system("pause");
		exit(1);
	}

	selector.add(listener);
	std::cout << "SERVER IS RUNNING YOO!! WE LISTENING ON PORT " << listener.getLocalPort() << "\n";
}

void Server::runServer(){
	
	while (true){
		if (selector.wait()) {

			//Test to see if the server is receiving a connection from a client. If so, add them to our map of clients
			if (selector.isReady(listener))
				listenForClients();

			//Otherwise, iterate over each client to send and receive messages from them.
			else
				processClients();
		}
	}

}

void Server::listenForClients(){
	//std::cout << "waiting for a connection...\n";

	auto client = std::make_unique<sf::TcpSocket>();
	std::string clientID;
	sf::Packet packet;

	//If a client has successfully connected to our server...
	if (listener.accept(*client) == sf::Socket::Done){
		clientJoined = true;

		//Read in the id sent by the client. Error handle to make sure it was received and extracted successfully.
		if (client->receive(packet) == sf::Socket::Done and packet >> clientID) {
			clientJoinedMessage = clientID + " has connected to the chat!\n";
			nameOfNewClient = clientID;
			std::cout << clientID << " has connected to the chat!\n";
		}
			
		// Add the new client to the clients list
		clients[clientID] = std::move(client);

		// Add the new client to the selector so that we will be notified when he sends something
		selector.add(*clients[clientID]);

		//After adding in the new client, notify the other one's that this new client has been added.
		notifyClientsOfNewClient();
	}
	else
		std::cout << "Client: " << client->getRemoteAddress() << " failed to connect :/\n";

}

void Server::processClients(){

	for (auto &client : clients) {

		//Check each client to see if they are ready to receive data
		if (selector.isReady(*client.second)) {
			sf::Packet receivePacket, sendPacket;
			
			//Check to see if any of the clients has sent a message to the server. If so, send that message to every
			//other client except for the client that sent it.
			if (client.second->receive(receivePacket) == sf::Socket::Done) {
				std::string message;
				receivePacket >> message;//Extract the message sent into a string.
				sendPacket << message;//Send the message into a packet to the other clients

				for (auto &client2 : clients) {
					//If the client in this for loop is not the same one has the client we are currently processing in
					//the outer loop, send the message.
					if(client2.first != client.first)
						client2.second->send(sendPacket);
				}
					
			}

		}
	}

	clientJoined = false;
}

void Server::notifyClientsOfNewClient() {
	for (auto &client : clients) {
		if (selector.isReady(*client.second)) {
			sf::Packet receivePacket, sendPacket;

			std::cout << "Sending out notification!\n";
			//Insert the message that a new client has joined the server into the packet, and send it to the clients.
			if (client.first != nameOfNewClient) {
				receivePacket << clientJoinedMessage;
				client.second->send(receivePacket);
			}
		}
	}
}