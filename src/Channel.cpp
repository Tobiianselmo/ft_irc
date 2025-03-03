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

Channel::Channel(const std::string &name, const std::string &topic)
{
	std::cout << "Channel constructor with name and topic called." << std::endl;
	_name = name;
	_password = "";
	_topic = topic;
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

// Getters

std::string Channel::getName() const { return this->_name; }
std::string Channel::getPassword() const { return this->_password; }
std::string Channel::getTopic() const { return this->_topic; }