#include "Server.hpp"

Server::Server(){}

Server::~Server(){}

Server::Server(const Server &data)
{
	*this = data;
}
Server &Server::operator=(const Server &data)
{
	if(this != &data)
	{
		this->_port = data._port;
		this->_password = data._password;
	}
	return(*this);
}
Server::Server(int port,const std::string &password)
{
	this->_port = port;
	this->_password = password;
}
void Server::initServer()
{
	
}