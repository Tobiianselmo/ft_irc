#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Server;

class Client
{
	private:
		int _clientSocket;
		std::string _nickName;
		std::string _userName;
		bool	isAuth;

		Server *_server;
	public:
		Client(const std::string &nick, const std::string &user, const Server &server);
		Client(int socket);
		~Client();
		void	setupClient();
};

#endif