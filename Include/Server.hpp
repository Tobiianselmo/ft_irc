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

class Server
{
	private:
			int _port;
			std::string _password;
			int _serverSocket;
			Server();
			Server(const Server &data);
			Server &operator=(const Server &data);
	public:
			Server(int port,const std::string &password);
			void setupServer();
			void handleConnections();
			~Server();
};
#endif