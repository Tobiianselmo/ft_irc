#include "../include/Channel.hpp"

Channel::Channel()
{
	std::cout << "Channel default constructor called." << std::endl;
	_name = "";
	_password = "";
	_topic = "";
	_users = 0;
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

