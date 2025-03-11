#include "../include/Channel.hpp"

Channel::Channel()
{
	std::cout << "Channel default constructor called." << std::endl;
	_name = "";
	_password = "";
	_topic = "";
	_users = 0;
	_usersLimit = 10;
	_inviteOnly = false;
	_moderate = false;
	_externMessages = false;
	_hasLimit = false;
	_hasPassword = false;

	// Falta inicializar _clients
}

Channel::~Channel()
{
	std::cout << "Channel default destructor called." << std::endl;
}

Channel::Channel(const std::string &name)
{
	std::cout << "Channel constructor with name called." << std::endl;
	_name = name;
	_password = "";
	_topic = "";
	_users = 0;
	_usersLimit = 10;
	_inviteOnly = false;
	_moderate = false;
	_externMessages = false;
	_hasLimit = false;
	_hasPassword = false;

	// Falta inicializar _clients
}

// Setters

void Channel::setName(std::string name) { this->_name = name; }
void Channel::setPassword(std::string password) { this->_password = password; }
void Channel::setTopic(std::string topic) { this->_topic = topic; }
void Channel::setHasPassword(bool value) { this->_hasPassword = value; }

// Getters

std::string Channel::getName() const { return this->_name; }
std::string Channel::getPassword() const { return this->_password; }
std::string Channel::getTopic() const { return this->_topic; }
std::vector<Client> Channel::getArrClients() const {return this->_clients;}

bool Channel::hasPassword() const { return this->_hasPassword; }

bool Channel::isClient(const Client &client)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getClientSocket() == client.getClientSocket())
			return true;
	}
	return false;
}

bool Channel::isClient(const std::string &name)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickName() == name)
			return true;
	}
	return false;
}

void Channel::addClient(const Client &client)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getClientSocket() == client.getClientSocket())
			return ;
	}
	_clients.push_back(client);
}

void Channel::addOperator(const Client &client)
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i].getClientSocket() == client.getClientSocket())
			return ;
	}
	_operators.push_back(client);
}
