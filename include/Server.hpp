#ifndef SERVER_HPP
#define SERVER_HPP


#include <algorithm>
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
#include <vector>
#include "Client.hpp"

class Client;

class Server
{
	private:
		int _port;
		int _serverSocket;
		std::string _password;
		Server(const Server &data);
		std::vector <struct pollfd >_fds; //añadir cuantos clientes vamos a manejar
		std::vector<Client *> _client;
	public:
		Server();
		Server(int port, const std::string &password);
		~Server();
		Server &operator=(const Server &data);

		int 	getPort() const;
		const std::string &getPassword() const;

		void	setupServer();
		void	handleConnections();
		void	newConnections();
		void 	eventMsg(std::vector<struct pollfd> &fds, int i);
		void	parsedInput(std::string str);
		void	checkCommand(std::vector<std::string> arr);
};
std::vector<std::string>	split(const std::string &str, char delimiter);
void						removeCarriageReturn(std::string &str);
void						checkNick(std::string str);
#endif