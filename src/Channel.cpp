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
	_hasTopic = false;
}

// Setters

void	Channel::setName(std::string name) { this->_name = name; }
void	Channel::setPassword(std::string password) { this->_password = password; }
void	Channel::setHasPassword(bool value) { this->_hasPassword = value; }
void	Channel::setTopic(std::string topic,bool val) { this->_topic = topic; this->_hasTopic = val; }
void	Channel::setMode(std::string mode, bool choice)
{
	if (mode == "+i" || mode == "-i")
		this->_inviteOnly = choice;
}

// Getters

std::string	Channel::getName() const { return this->_name; }
std::string	Channel::getPassword() const { return this->_password; }
std::string	Channel::getTopic() const { return this->_topic; }
bool		Channel::hasPassword() const { return this->_hasPassword; }
bool		Channel::hasTopic() const { return this->_hasTopic; }
bool		Channel::getInvite() const { return this->_inviteOnly; }

void		Channel::getMode() const
{
	std::cout << "Invite only Mode set to = " << this->_inviteOnly << std::endl;
}

Client		*Channel::getClient(std::string nick)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickName() == nick)
			return (&this->_clients[i]);
	}
	return (NULL);
}
std::vector<Client>	Channel::getArrClients() const { return this->_clients; }

// Bools

bool	Channel::isClient(const Client &client)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getClientSocket() == client.getClientSocket())
			return true;
	}
	return false;
}

bool	Channel::isClient(const std::string &name)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickName() == name)
			return true;
	}
	return false;
}

bool	Channel::isOperator(std::string nick) const
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i].getNickName() == nick)
			return (true);
	}
	return (false);
}

bool	Channel::isInvited(std::string nick) const
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		if (this->_invites[i].getNickName() == nick)
			return (true);
	}
	return (false);
}

// Add

void	Channel::addClient(const Client &client)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getClientSocket() == client.getClientSocket())
			return ;
	}
	_clients.push_back(client);
}

void	Channel::addOperator(const Client &client)
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i].getClientSocket() == client.getClientSocket())
			return ;
	}
	_operators.push_back(client);
}

void	Channel::addInvite(const Client &client)
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		if (this->_invites[i].getClientSocket() == client.getClientSocket())
			return ;
	}
	this->_invites.push_back(client);
}

// Deleters

// void	Channel::deleteClient(const Client &client)
// {
// 	int cont;

// 	for (size_t i = 0; i < this->_clients.size(); i++)
// 	{
// 		if (this->_clients[i].getNickName() == client.getNickName())
// 			cont = i;
// 	}
// 	this->_clients.erase(this->_clients.begin() + cont);
// }

void	Channel::deleteClient(const Client &client)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickName() == client.getNickName())
			this->_clients.erase(this->_clients.begin() + i);
	}
}

// void	Channel::deleteOperators(const Client &client)
// {
// 	int cont;

// 	for (size_t i = 0; i < this->_operators.size(); i++)
// 	{
// 		if (this->_operators[i].getNickName() == client.getNickName())
// 			cont = i;
// 	}
// 	this->_operators.erase(this->_operators.begin() + cont);
// }

void	Channel::deleteOperators(const Client &client)
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i].getNickName() == client.getNickName())
			this->_operators.erase(this->_operators.begin() + i);
	}
}

void	Channel::deleteInvited(const Client &client)
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		if (this->_invites[i].getNickName() == client.getNickName())
			this->_invites.erase(this->_invites.begin() + i);
	}
}

// Prints

void	Channel::printChannel()
{
	std::cout << "Channel: " << this->getName() << std::endl;
	std::cout << "- Password: " << this->getPassword() << std::endl;
	std::cout << "- Topic: " << this->getTopic() << std::endl;
	std::cout << "- Invite mode: ";
	this->getMode();
	std::cout << "Clients List: " << std::endl;
	this->printClients();
	std::cout << "Operators List: " << std::endl;
	this->printOperators();
	std::cout << "Invited List: " << std::endl;
	this->printInvited();
}

void	Channel::printClients()
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		std::cout << "Client Nº " << i << " " << this->_clients[i].getNickName() << std::endl;
	}
}

void	Channel::printInvited()
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		std::cout << "Invited " << i << " " << this->_invites[i].getNickName() << std::endl;
	}
}

void	Channel::printOperators()
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		std::cout << "operators " << this->_operators[i].getNickName() << std::endl;
	}
}