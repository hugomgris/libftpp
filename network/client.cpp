/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:00:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 14:53:54 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <errno.h>

Client::Client() : _socket(-1), _connected(false), _serverPort(0) {}

Client::~Client() {
	disconnect();
}

bool Client::_createSocket() {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket < 0) {
		return false;
	}

	// Set socket to non-blocking mode for async operation
	int flags = fcntl(_socket, F_GETFL, 0);
	if (flags < 0 || fcntl(_socket, F_SETFL, flags | O_NONBLOCK) < 0) {
		_closeSocket();
		return false;
	}

	return true;
}

void Client::_closeSocket() {
	if (_socket >= 0) {
		close(_socket);
		_socket = -1;
	}
}

void Client::connect(const std::string& address, const size_t& port) {
	if (_connected) {
		throw std::runtime_error("Client is already connected");
	}

	if (!_createSocket()) {
		throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
	}

	sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(static_cast<uint16_t>(port));

	if (inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0) {
		_closeSocket();
		throw std::runtime_error("Invalid address format: " + address);
	}

	// Temporarily set socket to blocking for connection
	int flags = fcntl(_socket, F_GETFL, 0);
	fcntl(_socket, F_SETFL, flags & ~O_NONBLOCK);

	if (::connect(_socket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) {
		_closeSocket();
		throw std::runtime_error("Failed to connect to " + address + ":" + std::to_string(port) + " - " + std::string(strerror(errno)));
	}

	// Set back to non-blocking
	fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

	_connected = true;
	_serverAddress = address;
	_serverPort = port;
}

void Client::disconnect() {
	if (_connected) {
		_closeSocket();
		_connected = false;
		_serverAddress.clear();
		_serverPort = 0;

		// Clear pending messages
		std::lock_guard<std::mutex> lock(_messageQueueMutex);
		while (!_receivedMessages.empty()) {
			_receivedMessages.pop();
		}
	}
}

void Client::defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action) {
	_messageActions[static_cast<int>(messageType)] = action;
}

void Client::send(const Message& message) {
	if (!_connected) {
		throw std::runtime_error("Client is not connected");
	}

	std::vector<uint8_t> serializedData = message.serialize();
	size_t totalSent = 0;
	size_t dataSize = serializedData.size();

	while (totalSent < dataSize) {
		ssize_t sent = ::send(_socket, serializedData.data() + totalSent, dataSize - totalSent, 0);
		
		if (sent < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				// Socket buffer is full, wait a bit and try again
				usleep(1000); // 1ms
				continue;
			} else {
				throw std::runtime_error("Failed to send message: " + std::string(strerror(errno)));
			}
		} else if (sent == 0) {
			throw std::runtime_error("Connection closed by server");
		}

		totalSent += sent;
	}
}

bool Client::_receiveData() {
	if (!_connected) {
		return false;
	}

	// Try to read header first (8 bytes: type + size)
	std::vector<uint8_t> headerBuffer(8);
	ssize_t headerReceived = recv(_socket, headerBuffer.data(), 8, MSG_PEEK);

	if (headerReceived < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return true; // No data available, but connection is still good
		} else {
			return false; // Connection error
		}
	} else if (headerReceived == 0) {
		return false; // Connection closed by server
	} else if (headerReceived < 8) {
		return true; // Partial header, wait for more data
	}

	// Extract message size from header
	uint32_t networkSize;
	std::memcpy(&networkSize, headerBuffer.data() + 4, sizeof(uint32_t));
	uint32_t messageSize = ntohl(networkSize);

	// Total message size = header + data
	size_t totalSize = 8 + messageSize;
	std::vector<uint8_t> fullBuffer(totalSize);

	// Read the complete message
	ssize_t totalReceived = recv(_socket, fullBuffer.data(), totalSize, 0);
	
	if (totalReceived < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return true;
		} else {
			return false;
		}
	} else if (totalReceived == 0) {
		return false;
	} else if (static_cast<size_t>(totalReceived) < totalSize) {
		return true; // Partial message, wait for more data
	}

	// Parse and queue the message
	try {
		Message receivedMessage = Message::deserialize(fullBuffer);
		std::lock_guard<std::mutex> lock(_messageQueueMutex);
		_receivedMessages.push(receivedMessage);
	} catch (const std::exception& e) {
		// Ignore malformed messages
		std::cerr << "Failed to parse received message: " << e.what() << std::endl;
	}

	return true;
}

void Client::update() {
	if (!_connected) {
		return;
	}

	// Receive new messages
	if (!_receiveData()) {
		// Connection lost
		disconnect();
		return;
	}

	// Process queued messages
	std::lock_guard<std::mutex> lock(_messageQueueMutex);
	while (!_receivedMessages.empty()) {
		Message message = _receivedMessages.front();
		_receivedMessages.pop();

		// Find and execute the action for this message type
		auto it = _messageActions.find(message.type());
		if (it != _messageActions.end() && it->second) {
			try {
				it->second(message);
			} catch (const std::exception& e) {
				std::cerr << "Error executing message action for type " << message.type() << ": " << e.what() << std::endl;
			}
		}
	}
}

bool Client::isConnected() const {
	return _connected;
}

const std::string& Client::getServerAddress() const {
	return _serverAddress;
}

size_t Client::getServerPort() const {
	return _serverPort;
}
