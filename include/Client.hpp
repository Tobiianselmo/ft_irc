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

		std::string	_buffer;

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
		void	setBuffer(char *aux);
		void	setAuth(bool val);
		void	setCorrectPass(bool val);

		// Getters
		const std::string	&getNickName() const;
		const std::string	&getUserName() const;
		const std::string	&getBuffer() const;
		int					getClientSocket() const;
		bool				isAuth() const;
		bool				hasCorrectPass() const;

		void				eraseBuffer();
};

#endif