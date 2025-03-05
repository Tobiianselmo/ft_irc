#include "../include/Client.hpp"

Client::Client() {}

Client::~Client()
{
/* 	if (_server)
		delete _server; */
}

Client::Client(const Client &other)
{
	*this = other;
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->_clientSocket = other._clientSocket;
		this->_isAuth = other._isAuth;
		this->_nickName = other._nickName;
		this->_server = other._server;
		this->_userName = other._userName;
	}
	return *this;
}

Client::Client(int socket,Server &server)
{
	this->_isAuth = false;
	this->_nickName = "";
	this->_userName = "";
	this->_clientSocket = socket;
	this->_server = &server;
}

Client::Client(const std::string &nick, const std::string &user, const Server &server)
{
	this->_nickName = nick;
	this->_userName = user;
	*_server = server;
}

// Setters

void	Client::setNickName(std::string nickname) { this->_nickName = nickname; }
void	Client::setUserName(std::string username) { this->_userName = username; }

// Getters

const std::string &Client::getNickName() const { return this->_nickName; }
const std::string &Client::getUserName() const { return this->_userName; }
int	Client::getClientSocket() const { return this->_clientSocket; }

// std::ostream &operator<<(std::ostream &output, const Client &other)
// {

// 	return output;
// }