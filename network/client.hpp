/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:00:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 14:53:54 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <functional>
# include <map>
# include <vector>
# include <queue>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <mutex>

# include "message.hpp"

class Client {
	private:
		int _socket;
		bool _connected;
		std::string _serverAddress;
		size_t _serverPort;
		
		// Message handling
		std::map<int, std::function<void(const Message&)>> _messageActions;
		std::queue<Message> _receivedMessages;
		std::mutex _messageQueueMutex;
		
		// Private helper methods
		bool _createSocket();
		void _closeSocket();
		bool _receiveData();
		Message _parseMessage(const std::vector<uint8_t>& data);

	public:
		Client();
		~Client();

		// Copy constructor and assignment operator (deleted for socket safety)
		Client(const Client&) = delete;
		Client& operator=(const Client&) = delete;

		// Main interface methods
		void connect(const std::string& address, const size_t& port);
		void disconnect();
		void defineAction(const Message::Type& messageType, const std::function<void(const Message& msg)>& action);
		void send(const Message& message);
		void update();

		// Utility methods
		bool isConnected() const;
		const std::string& getServerAddress() const;
		size_t getServerPort() const;
};

#endif
