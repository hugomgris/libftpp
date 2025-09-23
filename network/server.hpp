/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:00:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/09/23 15:07:53 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <functional>
# include <map>
# include <vector>
# include <queue>
# include <set>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/select.h>
# include <mutex>

# include "message.hpp"

class Server {
	private:
		struct ClientInfo {
			int socket;
			sockaddr_in address;
			std::queue<Message> receivedMessages;
			std::vector<uint8_t> receiveBuffer;
			size_t expectedMessageSize;
			bool headerReceived;
			
			ClientInfo(int sock, const sockaddr_in& addr) 
				: socket(sock), address(addr), expectedMessageSize(0), headerReceived(false) {}
		};

		int _serverSocket;
		bool _running;
		size_t _port;
		std::map<long long, ClientInfo> _clients;
		long long _nextClientID;
		
		// Message handling
		std::map<int, std::function<void(long long&, const Message&)>> _messageActions;
		mutable std::mutex _clientsMutex;
		mutable std::mutex _messageActionsMutex;
		
		// Private helper methods
		bool _createServerSocket();
		void _closeServerSocket();
		void _acceptNewClients();
		void _processClientData();
		void _processClientMessages(long long clientID, ClientInfo& client);
		void _disconnectClient(long long clientID);
		bool _receiveFromClient(long long clientID, ClientInfo& client);
		void _sendToClient(const Message& message, long long clientID);

	public:
		Server();
		~Server();

		// Copy constructor and assignment operator (deleted for socket safety)
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		// Main interface methods
		void start(const size_t& port);
		void stop();
		void defineAction(const Message::Type& messageType, 
		                 const std::function<void(long long&, const Message&)>& action);
		void sendTo(const Message& message, long long clientID);
		void sendToArray(const Message& message, const std::vector<long long>& clientIDs);
		void sendToAll(const Message& message);
		void update();

		// Utility methods
		bool isRunning() const;
		size_t getPort() const;
		std::vector<long long> getConnectedClients() const;
		size_t getClientCount() const;
};

#endif
