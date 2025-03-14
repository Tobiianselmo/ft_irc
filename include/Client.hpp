#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Server;

class Client
{
	private:
		int			_clientSocket;
		std::string	_nickName;
		std::string	_userName;
		bool		_isAuth;
		bool		_hasCorrectPass;

		Server *_server;
	public:
		Client();
		~Client();
		Client(const Client &other);
		Client &operator=(const Client &other);
		Client(int socket, Server &server);

		// Setters
		void	setNickName(std::string nickname);
		void	setUserName(std::string username);
		void	setAuth(bool val);
		void	setCorrectPass(bool val);

		// Getters
		const std::string	&getNickName() const;
		const std::string	&getUserName() const;
		int					getClientSocket() const;
		bool				isAuth() const;
		bool				hasCorrectPass() const;
};

#endif