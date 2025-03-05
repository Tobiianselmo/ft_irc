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
		Client(const std::string &nick, const std::string &user, const Server &server);
		Client(int socket,Server &server);

		void	setNickName(std::string nickname);
		void	setUserName(std::string username);

		const std::string &getNickName() const;
		const std::string &getUserName() const;
		int	getClientSocket() const;
};

// std::ostream &operator<<(std::ostream &output, const Client &other);

#endif