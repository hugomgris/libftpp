/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:00:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 15:10:54 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <errno.h>
#include <algorithm>

Server::Server() : _serverSocket(-1), _running(false), _port(0), _nextClientID(1) {}

Server::~Server() {
	stop();
}

bool Server::_createServerSocket() {
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0) {
		return false;
	}

	// Set socket options
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		_closeServerSocket();
		return false;
	}

	// Set non-blocking mode
	int flags = fcntl(_serverSocket, F_GETFL, 0);
	if (flags < 0 || fcntl(_serverSocket, F_SETFL, flags | O_NONBLOCK) < 0) {
		_closeServerSocket();
		return false;
	}

	return true;
}

void Server::_closeServerSocket() {
	if (_serverSocket >= 0) {
		close(_serverSocket);
		_serverSocket = -1;
	}
}

void Server::start(const size_t& port) {
	if (_running) {
		throw std::runtime_error("Server is already running");
	}

	if (!_createServerSocket()) {
		throw std::runtime_error("Failed to create server socket: " + std::string(strerror(errno)));
	}

	sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(static_cast<uint16_t>(port));

	if (bind(_serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
		_closeServerSocket();
		throw std::runtime_error("Failed to bind to port " + std::to_string(port) + ": " + std::string(strerror(errno)));
	}

	if (listen(_serverSocket, SOMAXCONN) < 0) {
		_closeServerSocket();
		throw std::runtime_error("Failed to listen on socket: " + std::string(strerror(errno)));
	}

	_running = true;
	_port = port;
}

void Server::stop() {
	if (_running) {
		_running = false;
		
		// Disconnect all clients
		std::lock_guard<std::mutex> lock(_clientsMutex);
		for (auto& [clientID, client] : _clients) {
			close(client.socket);
		}
		_clients.clear();
		
		_closeServerSocket();
		_port = 0;
		_nextClientID = 1;
	}
}

void Server::_acceptNewClients() {
	sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	
	int clientSocket = accept(_serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
	if (clientSocket < 0) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			std::cerr << "Accept failed: " << strerror(errno) << std::endl;
		}
		return;
	}

	// Set client socket to non-blocking
	int flags = fcntl(clientSocket, F_GETFL, 0);
	if (flags >= 0) {
		fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
	}

	// Add new client
	std::lock_guard<std::mutex> lock(_clientsMutex);
	long long clientID = _nextClientID++;
	_clients.emplace(clientID, ClientInfo(clientSocket, clientAddr));
	
	std::cout << "Client " << clientID << " connected from " 
	          << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
}

bool Server::_receiveFromClient(long long, ClientInfo& client) {
	std::vector<uint8_t> tempBuffer(4096);
	ssize_t bytesReceived = recv(client.socket, tempBuffer.data(), tempBuffer.size(), 0);
	
	if (bytesReceived < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return true; // No data available
		}
		return false; // Error
	} else if (bytesReceived == 0) {
		return false; // Client disconnected
	}

	// Add received data to client's buffer
	client.receiveBuffer.insert(client.receiveBuffer.end(), 
	                           tempBuffer.begin(), tempBuffer.begin() + bytesReceived);
	
	return true;
}

void Server::_processClientMessages(long long clientID, ClientInfo& client) {
	while (true) {
		if (!client.headerReceived) {
			if (client.receiveBuffer.size() < 8) {
				break; // Need more data for header
			}
			
			// Extract message size from header
			uint32_t networkSize;
			std::memcpy(&networkSize, client.receiveBuffer.data() + 4, sizeof(uint32_t));
			client.expectedMessageSize = 8 + ntohl(networkSize); // header + data
			client.headerReceived = true;
		}
		
		// Check for complete message
		if (client.receiveBuffer.size() < client.expectedMessageSize) {
			break; // Need more data for message
		}
		
		// Extract the complete message
		std::vector<uint8_t> messageData(client.receiveBuffer.begin(), 
		                                client.receiveBuffer.begin() + client.expectedMessageSize);
		
		// Remove processed data from buffer
		client.receiveBuffer.erase(client.receiveBuffer.begin(), 
		                          client.receiveBuffer.begin() + client.expectedMessageSize);
		
		// Reset for next message
		client.headerReceived = false;
		client.expectedMessageSize = 0;
		
		// Parse and queue the message
		try {
			Message receivedMessage = Message::deserialize(messageData);
			client.receivedMessages.push(receivedMessage);
		} catch (const std::exception& e) {
			std::cerr << "Failed to parse message from client " << clientID << ": " << e.what() << std::endl;
		}
	}
}

void Server::_processClientData() {
	std::lock_guard<std::mutex> lock(_clientsMutex);
	auto it = _clients.begin();
	
	while (it != _clients.end()) {
		long long clientID = it->first;
		ClientInfo& client = it->second;
		
		// Try to receive data from client
		if (!_receiveFromClient(clientID, client)) {
			// Client disconnection / error
			std::cout << "Client " << clientID << " disconnected" << std::endl;
			close(client.socket);
			it = _clients.erase(it);
			continue;
		}

		_processClientMessages(clientID, client);
		++it;
	}
}

void Server::_disconnectClient(long long clientID) {
	std::lock_guard<std::mutex> lock(_clientsMutex);
	auto it = _clients.find(clientID);
	if (it != _clients.end()) {
		close(it->second.socket);
		_clients.erase(it);
		std::cout << "Client " << clientID << " forcibly disconnected" << std::endl;
	}
}

void Server::defineAction(const Message::Type& messageType, 
                         const std::function<void(long long&, const Message&)>& action) {
	std::lock_guard<std::mutex> lock(_messageActionsMutex);
	_messageActions[static_cast<int>(messageType)] = action;
}

void Server::_sendToClient(const Message& message, long long clientID) {
	std::lock_guard<std::mutex> lock(_clientsMutex);
	auto it = _clients.find(clientID);
	if (it == _clients.end()) {
		return; // Client not found
	}
	
	std::vector<uint8_t> serializedData = message.serialize();
	size_t totalSent = 0;
	size_t dataSize = serializedData.size();
	
	while (totalSent < dataSize) {
		ssize_t sent = send(it->second.socket, serializedData.data() + totalSent, 
		                   dataSize - totalSent, MSG_NOSIGNAL);
		
		if (sent < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				usleep(1000);
				continue;
			} else {
				/// Client disconnection / error
				std::cout << "Failed to send to client " << clientID << ", disconnecting" << std::endl;
				close(it->second.socket);
				_clients.erase(it);
				break;
			}
		} else if (sent == 0) {
			// Connection closed
			close(it->second.socket);
			_clients.erase(it);
			break;
		}
		
		totalSent += sent;
	}
}

void Server::sendTo(const Message& message, long long clientID) {
	if (!_running) {
		throw std::runtime_error("Server is not running");
	}
	_sendToClient(message, clientID);
}

void Server::sendToArray(const Message& message, const std::vector<long long>& clientIDs) {
	if (!_running) {
		throw std::runtime_error("Server is not running");
	}
	
	for (long long clientID : clientIDs) {
		_sendToClient(message, clientID);
	}
}

void Server::sendToAll(const Message& message) {
	if (!_running) {
		throw std::runtime_error("Server is not running");
	}
	
	std::lock_guard<std::mutex> lock(_clientsMutex);
	for (const auto& [clientID, client] : _clients) {
		// Release lock temporarily for sending
		_clientsMutex.unlock();
		_sendToClient(message, clientID);
		_clientsMutex.lock();
	}
}

void Server::update() {
	if (!_running) {
		return;
	}
	
	// Accept new clients
	_acceptNewClients();
	
	// Process data from existing clients
	_processClientData();
	
	// Process messages from all clients
	std::vector<std::pair<long long, Message>> messagesToProcess;
	
	{
		std::lock_guard<std::mutex> lock(_clientsMutex);
		for (auto& [clientID, client] : _clients) {
			while (!client.receivedMessages.empty()) {
				messagesToProcess.emplace_back(clientID, client.receivedMessages.front());
				client.receivedMessages.pop();
			}
		}
	}
	
	// Execute actions for received messages
	std::lock_guard<std::mutex> lock(_messageActionsMutex);
	for (auto& [clientID, message] : messagesToProcess) {
		auto it = _messageActions.find(message.type());
		if (it != _messageActions.end() && it->second) {
			try {
				it->second(clientID, message);
			} catch (const std::exception& e) {
				std::cerr << "Error executing message action for type " << message.type() 
				         << " from client " << clientID << ": " << e.what() << std::endl;
			}
		}
	}
}

bool Server::isRunning() const {
	return _running;
}

size_t Server::getPort() const {
	return _port;
}

std::vector<long long> Server::getConnectedClients() const {
	std::lock_guard<std::mutex> lock(_clientsMutex);
	std::vector<long long> clientIDs;
	clientIDs.reserve(_clients.size());
	
	for (const auto& [clientID, client] : _clients) {
		clientIDs.push_back(clientID);
	}
	
	return clientIDs;
}

size_t Server::getClientCount() const {
	std::lock_guard<std::mutex> lock(_clientsMutex);
	return _clients.size();
}
