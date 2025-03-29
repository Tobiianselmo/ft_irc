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
	_hasLimit = true;
	_hasPassword = false;
	_allowedTopic = false;
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
	_allowedTopic = false;
}

// Setters

void	Channel::setName(std::string name) { this->_name = name; }
void	Channel::setPassword(std::string password) { this->_password = password; }
void	Channel::setHasPassword(bool value) { this->_hasPassword = value; }
void	Channel::setHasLimit(bool value) { this->_hasLimit = value; }
void	Channel::setLimit(int limit) { this->_usersLimit = limit; }
void	Channel::setInviteOnly(bool choice) { this->_inviteOnly = choice; }
void	Channel::setAllowedTopic(bool choice) { this->_allowedTopic = choice; }
void	Channel::setTopic(std::string topic,bool val,std::string topicCreate)
{
	this->_topicCreate = topicCreate;
	std::stringstream aux;	
	this->_topic = topic;
	this->_hasTopic = val;
	std::time_t time = std::time(NULL);
	aux << time;
	this->_nowTime = aux.str();
}

// Getters

std::string Channel::getTopicCreate() const {return this->_topicCreate;}
std::string Channel::getNowTime() const {return this->_nowTime;}
std::string	Channel::getName() const { return this->_name; }
std::string	Channel::getPassword() const { return this->_password; }
std::string	Channel::getTopic() const { return this->_topic; }
bool		Channel::hasPassword() const { return this->_hasPassword; }
bool		Channel::hasTopic() const { return this->_hasTopic; }
bool		Channel::getInvite() const { return this->_inviteOnly; }
int			Channel::getUserSize() const { return this->_users; }
int			Channel::getUsersLimit() const { return this->_usersLimit; }
int			Channel::getOperatorsSize() const { return this->_operators.size(); }
bool		Channel::hasLimit() const { return this->_hasLimit; }
bool		Channel::getAllowedTopic() const { return this->_allowedTopic; }

void		Channel::getMode() const
{
	std::cout << "invited false" << std::endl;
}

Client		*Channel::getClient(std::string nick)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getNickName() == nick)
			return (this->_clients[i]);
	}
	return (NULL);
}
std::vector<Client *>	Channel::getArrClients() const { return this->_clients; }

// Bools

bool	Channel::isClient(const Client &client)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getClientSocket() == client.getClientSocket())
			return true;
	}
	return false;
}

bool	Channel::isClient(const std::string &name)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getNickName() == name)
			return true;
	}
	return false;
}

bool	Channel::isOperator(std::string nick) const
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i]->getNickName() == nick)
			return (true);
	}
	return (false);
}

bool	Channel::isInvited(std::string nick) const
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		if (this->_invites[i]->getNickName() == nick)
			return (true);
	}
	return (false);
}

// Add

void	Channel::addClient(Client *client)
{
	for (size_t i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getClientSocket() == client->getClientSocket())
			return ;
	}
	_clients.push_back(client);
	this->_users = _clients.size();
}

void	Channel::addOperator(Client *client)
{
	for (size_t i = 0; i < _operators.size(); i++)
	{
		if (_operators[i]->getClientSocket() == client->getClientSocket())
			return ;
	}
	_operators.push_back(client);
}

void	Channel::addInvite(Client *client)
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		if (this->_invites[i]->getClientSocket() == client->getClientSocket())
			return ;
	}
	this->_invites.push_back(client);
}

// Deleters

void	Channel::deleteClient(Client *client)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getNickName() == client->getNickName())
			this->_clients.erase(this->_clients.begin() + i);
	}
	this->_users = _clients.size();
	deleteOperators(client);
	deleteInvited(client);
	// actualizar a todo el canal aca mismo (pasar la lista de usuarios).
	// sendMsgToChannel(this, ...); 
}

void	Channel::deletePassword() { this->_password.erase(); }

void	Channel::deleteOperators(Client *client)
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i]->getNickName() == client->getNickName())
			this->_operators.erase(this->_operators.begin() + i);
	}
}

void	Channel::deleteInvited(Client *client)
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		if (this->_invites[i]->getNickName() == client->getNickName())
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
		std::cout << "Client Nº " << i << " " << this->_clients[i]->getNickName() << std::endl;
	}
}

void	Channel::printInvited()
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		std::cout << "Invited " << i << " " << this->_invites[i]->getNickName() << std::endl;
	}
}

void	Channel::printOperators()
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		std::cout << "operators " << this->_operators[i]->getNickName() << std::endl;
	}
}