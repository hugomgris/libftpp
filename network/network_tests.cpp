/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_tests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:10:37 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 15:09:13 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <thread>
#include <chrono>

#include "network.hpp"
#include "../colors.h"

void testMessage() {
	std::cout << YEL << "\n=== Testing message ===" << RESET << std::endl;

	Message msg(42);

	msg << int(123) << double(3.14159) << bool(true) << std::string("Hello Network!");

	std::cout << "Message type: " << msg.type() << std::endl;
	std::cout << "Message size: " << msg.getDataSize() << " bytes" << std::endl;

	int i; double d; bool b; std::string s;
	msg >> i >> d >> b >> s;

	std::cout << "Read: " << i << ", " << d << ", " << b << ", " << s << std::endl;
	
	auto serialized = msg.serialize();
	std::cout << "Serialized size: " << serialized.size() << " bytes" << std::endl;
	
	try {
		Message restored = Message::deserialize(serialized);
		std::cout << "Deserialized message type: " << restored.type() << std::endl;
	} catch (const std::exception& e) {
		std::cout << "Deserialization error: " << e.what() << std::endl;
	}

	std::cout << GRN << "Message tests completed!" << RESET << std::endl;
}

void testMessageTypes() {
	std::cout << YEL << "\n=== Testing Message Types ===" << RESET << std::endl;

	// Test different message types using the new enum
	Message connectMsg(Message::CONNECT_REQUEST);
	connectMsg << std::string("test_user") << static_cast<uint32_t>(12345);
	
	Message chatMsg(Message::CHAT_MESSAGE);
	chatMsg << std::string("Hello World!") << static_cast<int32_t>(42);
	
	Message heartbeatMsg(Message::HEARTBEAT);
	heartbeatMsg << static_cast<uint32_t>(1234567890);

	std::cout << "Connect message type: " << connectMsg.type() << std::endl;
	std::cout << "Chat message type: " << chatMsg.type() << std::endl;
	std::cout << "Heartbeat message type: " << heartbeatMsg.type() << std::endl;

	// Test serialization/deserialization
	auto serialized = chatMsg.serialize();
	Message deserialized = Message::deserialize(serialized);
	
	std::string text;
	int32_t number;
	deserialized >> text >> number;
	
	std::cout << "Deserialized text: '" << text << "', number: " << number << std::endl;
	std::cout << GRN << "Message types tests completed!" << RESET << std::endl;
}

void testClientBasicFunctionality() {
	std::cout << YEL << "\n=== Testing Client Basic Functionality ===" << RESET << std::endl;

	Client client;
	
	// Test initial state
	std::cout << "Initial connection state: " << (client.isConnected() ? "Connected" : "Disconnected") << std::endl;
	
	// Test defineAction
	bool messageReceived = false;
	client.defineAction(Message::CHAT_MESSAGE, [&messageReceived](const Message& msg) {
		std::cout << GRN << "Received chat message with type: " << msg.type() << RESET << std::endl;
		messageReceived = true;
	});
	
	client.defineAction(Message::HEARTBEAT, [](const Message&) {
		std::cout << BLU << "Received heartbeat message" << RESET << std::endl;
	});

	std::cout << "Message actions defined successfully" << std::endl;
	std::cout << GRN << "Client basic functionality tests completed!" << RESET << std::endl;
}

void testClientConnectionFailure() {
	std::cout << YEL << "\n=== Testing Client Connection Failure ===" << RESET << std::endl;

	Client client;
	
	try {
		// Try to connect to a non-existent server
		client.connect("127.0.0.1", 9999);
		std::cout << RED << "ERROR: Connection should have failed!" << RESET << std::endl;
	} catch (const std::exception& e) {
		std::cout << GRN << "Expected connection failure: " << e.what() << RESET << std::endl;
	}
	
	// Test sending without connection
	try {
		Message msg(Message::CHAT_MESSAGE);
		msg << std::string("test");
		client.send(msg);
		std::cout << RED << "ERROR: Send should have failed!" << RESET << std::endl;
	} catch (const std::exception& e) {
		std::cout << GRN << "Expected send failure: " << e.what() << RESET << std::endl;
	}

	std::cout << GRN << "Client connection failure tests completed!" << RESET << std::endl;
}

void testClientMessageUpdate() {
	std::cout << YEL << "\n=== Testing Client Message Update ===" << RESET << std::endl;

	Client client;
	
	// Test update on disconnected client
	client.update(); // Should not crash
	std::cout << "Update called on disconnected client - no issues" << std::endl;
	
	// Test multiple actions
	int chatCount = 0;
	int heartbeatCount = 0;
	
	client.defineAction(Message::CHAT_MESSAGE, [&chatCount](const Message&) {
		chatCount++;
		std::cout << "Chat message action #" << chatCount << " defined" << std::endl;
	});
	
	client.defineAction(Message::HEARTBEAT, [&heartbeatCount](const Message&) {
		heartbeatCount++;
		std::cout << "Heartbeat action #" << heartbeatCount << " defined" << std::endl;
	});
	
	std::cout << "Multiple actions defined successfully" << std::endl;
	std::cout << GRN << "Client message update tests completed!" << RESET << std::endl;
}

void testServerBasicFunctionality() {
	std::cout << YEL << "\n=== Testing Server Basic Functionality ===" << RESET << std::endl;

	Server server;
	
	// Test initial state
	std::cout << "Initial running state: " << (server.isRunning() ? "Running" : "Stopped") << std::endl;
	std::cout << "Initial port: " << server.getPort() << std::endl;
	std::cout << "Initial client count: " << server.getClientCount() << std::endl;
	
	// Test defineAction
	bool messageReceived = false;
	server.defineAction(Message::CONNECT_REQUEST, [&messageReceived](long long& clientID, const Message&) {
		std::cout << GRN << "Server received connect request from client: " << clientID << RESET << std::endl;
		messageReceived = true;
	});
	
	server.defineAction(Message::CHAT_MESSAGE, [](long long& clientID, const Message&) {
		std::cout << BLU << "Server received chat message from client: " << clientID << RESET << std::endl;
	});

	std::cout << "Server message actions defined successfully" << std::endl;
	std::cout << GRN << "Server basic functionality tests completed!" << RESET << std::endl;
}

void testServerStartStop() {
	std::cout << YEL << "\n=== Testing Server Start/Stop ===" << RESET << std::endl;

	Server server;
	
	try {
		// Test starting server on a common test port
		server.start(8080);
		std::cout << GRN << "Server started on port 8080" << RESET << std::endl;
		std::cout << "Running state: " << (server.isRunning() ? "Running" : "Stopped") << std::endl;
		std::cout << "Port: " << server.getPort() << std::endl;
		
		// Test update while running
		server.update(); // Should not crash
		std::cout << "Server update called successfully" << std::endl;
		
		// Test stopping
		server.stop();
		std::cout << GRN << "Server stopped successfully" << RESET << std::endl;
		std::cout << "Running state: " << (server.isRunning() ? "Running" : "Stopped") << std::endl;
		
	} catch (const std::exception& e) {
		std::cout << YEL << "Server start failed (port may be in use): " << e.what() << RESET << std::endl;
		
		// Try with a different port
		try {
			server.start(8081);
			std::cout << GRN << "Server started on alternate port 8081" << RESET << std::endl;
			server.stop();
			std::cout << GRN << "Server stopped successfully" << RESET << std::endl;
		} catch (const std::exception& e2) {
			std::cout << RED << "Server failed to start on alternate port: " << e2.what() << RESET << std::endl;
		}
	}

	std::cout << GRN << "Server start/stop tests completed!" << RESET << std::endl;
}

void testServerDoubleStart() {
	std::cout << YEL << "\n=== Testing Server Double Start ===" << RESET << std::endl;

	Server server;
	
	try {
		server.start(8082);
		std::cout << GRN << "Server started on port 8082" << RESET << std::endl;
		
		// Try to start again - should fail
		try {
			server.start(8083);
			std::cout << RED << "ERROR: Second start should have failed!" << RESET << std::endl;
		} catch (const std::exception& e) {
			std::cout << GRN << "Expected double start failure: " << e.what() << RESET << std::endl;
		}
		
		server.stop();
	} catch (const std::exception& e) {
		std::cout << YEL << "Server start failed (port may be in use): " << e.what() << RESET << std::endl;
	}

	std::cout << GRN << "Server double start tests completed!" << RESET << std::endl;
}

void testServerClientManagement() {
	std::cout << YEL << "\n=== Testing Server Client Management ===" << RESET << std::endl;

	Server server;
	
	// Test getting clients when server is not running
	auto clients = server.getConnectedClients();
	std::cout << "Clients when server stopped: " << clients.size() << std::endl;
	
	try {
		server.start(8084);
		std::cout << GRN << "Server started for client management test" << RESET << std::endl;
		
		// Test sending to non-existent client
		try {
			Message msg(Message::CHAT_MESSAGE);
			msg << std::string("test");
			server.sendTo(msg, 99999);
			std::cout << "Send to non-existent client completed (no error expected)" << std::endl;
		} catch (const std::exception& e) {
			std::cout << "Send to non-existent client error: " << e.what() << std::endl;
		}
		
		// Test broadcast with no clients
		Message broadcastMsg(Message::HEARTBEAT);
		server.sendToAll(broadcastMsg);
		std::cout << "Broadcast to empty client list completed" << std::endl;
		
		server.stop();
	} catch (const std::exception& e) {
		std::cout << YEL << "Server start failed (port may be in use): " << e.what() << RESET << std::endl;
	}

	std::cout << GRN << "Server client management tests completed!" << RESET << std::endl;
}

void testClientServerIntegration() {
	std::cout << YEL << "\n=== Testing Client-Server Integration ===" << RESET << std::endl;

	Server server;
	Client client;
	
	try {
		// Start server
		server.start(8085);
		std::cout << GRN << "Integration test server started on port 8085" << RESET << std::endl;
		
		// Set up server message handling
		bool serverReceivedMessage = false;
		server.defineAction(Message::CONNECT_REQUEST, [&serverReceivedMessage](long long& clientID, const Message&) {
			std::cout << GRN << "Server: Client " << clientID << " connected!" << RESET << std::endl;
			serverReceivedMessage = true;
		});
		
		server.defineAction(Message::CHAT_MESSAGE, [&server](long long& clientID, const Message&) {
			std::cout << GRN << "Server: Received chat from client " << clientID << RESET << std::endl;
			
			// Echo back to client
			Message response(Message::CHAT_MESSAGE);
			response << std::string("Echo from server");
			server.sendTo(response, clientID);
		});
		
		// Set up client message handling
		bool clientReceivedEcho = false;
		client.defineAction(Message::CHAT_MESSAGE, [&clientReceivedEcho](const Message&) {
			std::cout << GRN << "Client: Received echo from server!" << RESET << std::endl;
			clientReceivedEcho = true;
		});
		
		// Wait for server to be ready
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		// Connect client
		try {
			client.connect("127.0.0.1", 8085);
			std::cout << GRN << "Client connected to server!" << RESET << std::endl;
			
			// Send connection request
			Message connectMsg(Message::CONNECT_REQUEST);
			connectMsg << std::string("test_client");
			client.send(connectMsg);
			
			// Process messages on both ends
			for (int i = 0; i < 10; ++i) {
				server.update();
				client.update();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			
			// Send chat message
			Message chatMsg(Message::CHAT_MESSAGE);
			chatMsg << std::string("Hello server!");
			client.send(chatMsg);
			
			// Process more messages
			for (int i = 0; i < 10; ++i) {
				server.update();
				client.update();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			
			std::cout << "Client count on server: " << server.getClientCount() << std::endl;
			auto connectedClients = server.getConnectedClients();
			std::cout << "Connected client IDs: ";
			for (auto id : connectedClients) {
				std::cout << id << " ";
			}
			std::cout << std::endl;
			
			client.disconnect();
			std::cout << GRN << "Client disconnected" << RESET << std::endl;
			
		} catch (const std::exception& e) {
			std::cout << RED << "Client connection failed: " << e.what() << RESET << std::endl;
		}
		
		server.stop();
		std::cout << GRN << "Integration test server stopped" << RESET << std::endl;
		
	} catch (const std::exception& e) {
		std::cout << YEL << "Integration test failed (port may be in use): " << e.what() << RESET << std::endl;
	}

	std::cout << GRN << "Client-Server integration tests completed!" << RESET << std::endl;
}

int main(void) {
	std::cout << CYN << "====== NETWORK tests ======" << RESET << std::endl;

	testMessage();
	testMessageTypes();
	testClientBasicFunctionality();
	testClientConnectionFailure();
	testClientMessageUpdate();
	testServerBasicFunctionality();
	testServerStartStop();
	testServerDoubleStart();
	testServerClientManagement();
	testClientServerIntegration();

	std::cout << GRN << "\nAll network tests completed successfully!" << RESET << std::endl;

	return 0;
}