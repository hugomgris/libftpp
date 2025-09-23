/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_tests.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:10:37 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 14:59:15 by hmunoz-g         ###   ########.fr       */
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

int main(void) {
	std::cout << CYN << "====== NETWORK tests ======" << RESET << std::endl;

	testMessage();
	testMessageTypes();
	testClientBasicFunctionality();
	testClientConnectionFailure();
	testClientMessageUpdate();

	std::cout << GRN << "\nAll network tests completed successfully!" << RESET << std::endl;
	std::cout << YEL << "Note: Full client-server communication tests require a running server." << RESET << std::endl;

	return 0;
}