#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cstring>
#include <poll.h>
#include "Client.hpp"

class Client;

class Server
{
	private:
		int _port;
		int _serverSocket;
		std::string _password;
		Server();
		Server(const Server &data);
	public:
		Server(int port, const std::string &password);
		~Server();
		Server &operator=(const Server &data);

		int 	getPort() const;
		const std::string &getPassword() const;

		void	setupServer();
		void	handleConnections();
};

#endif