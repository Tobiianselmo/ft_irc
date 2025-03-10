
#include "../include/Server.hpp"

Client	*Server::getClient(std::string nick)
{
	for (size_t i = 1; i < this->_fds.size(); i++)
	{
		if (_clientsMap[_fds[i].fd]->getNickName() == nick)
			return (_clientsMap[_fds[i].fd]);
	}
	return (NULL);
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

void	Channel::addInvite(const Client &client)
{
	for (size_t i = 0; i < this->_invites.size(); i++)
	{
		if (this->_invites[i].getClientSocket() == client.getClientSocket())
			return ;
	}
	this->_invites.push_back(client);
}

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

int	Server::inviteCommand(std::string line, Client &client)
{
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() != 3)
		return (ERR_NEEDMOREPARAMS);
	Channel	*tmp = getChannel(parameters[2]);
	if (!tmp)
		return (ERR_NOSUCHCHANNEL);
	if (tmp->isOperator(client.getNickName()) == false)
		return (ERR_NOOPERHOST);
	Client	*clientTmp = this->getClient(parameters[1]);
	if (!clientTmp)
		return (ERR_NOSUCHNICK);
	tmp->addInvite(*clientTmp);
	tmp->printInvited();
	return (0);
}
