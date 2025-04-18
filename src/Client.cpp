#include "../include/Client.hpp"

Client::Client() {}

Client::~Client()
{
	// std::cout << "Client default destructor called." << std::endl;
	close(this->_clientSocket);
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
		this->_isNick = other._isNick;
		this->_nickName = other._nickName;
		this->_server = other._server;
		this->_userName = other._userName;
	}
	return *this;
}

Client::Client(int socket, Server &server)
{
	this->_isAuth = false;
	this->_isNick = false;
	this->_hasCorrectPass = false;
	this->_nickName = "";
	this->_userName = "";
	this->_clientSocket = socket;
	this->_server = &server;
}

// Setters

void	Client::setNickName(std::string nickname) { this->_nickName = nickname; }
void	Client::setUserName(std::string username) { this->_userName = username; }
void	Client::setAuth(bool val) { this->_isAuth = val; }
void	Client::setIsNick(bool val) { this->_isNick = val; }
void	Client::setCorrectPass(bool val) { this->_hasCorrectPass = val; }
void	Client::setBuffer(char *aux) { this->_buffer.append(aux); }

// Getters

const std::string	&Client::getNickName() const { return this->_nickName; }
const std::string	&Client::getUserName() const { return this->_userName; }
const std::string	&Client::getBuffer() const { return this->_buffer; }
int					Client::getClientSocket() const { return this->_clientSocket; }
bool				Client::getIsNick() const { return this->_isNick; }
bool				Client::isAuth() const { return this->_isAuth; }
bool				Client::hasCorrectPass() const { return this->_hasCorrectPass; }

void				Client::eraseBuffer() { this->_buffer.erase(); }
