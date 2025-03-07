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
		bool	_isAuth;

		Server *_server;
	public:
		Client();
		~Client();
		Client &operator=(const Client &other);
		Client(const Client &other);
		Client(int socket, Server &server);

		void	setNickName(std::string nickname);
		void	setUserName(std::string username);
		void	setAuth(bool val);

		const std::string	&getNickName() const;
		const std::string	&getUserName() const;
		int	getClientSocket() const;
		bool	isAuth() const;
};

#endif