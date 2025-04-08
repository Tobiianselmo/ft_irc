#include "../include/Server.hpp"

void	sendMsgToChannel(std::string msg, int sendTo, t_data &cmd)
{
	std::vector<Client *> clientsList = cmd.channel->getArrClients();

	for (size_t i = 0; i < clientsList.size(); i++)
	{
		if (sendTo == ALL_CHANNEL || sendTo == ONLY_OPERATORS)
		{
			if (sendTo == ONLY_OPERATORS)
			{
				if (cmd.channel->isOperator(clientsList[i]->getNickName()) == true)
					send(clientsList[i]->getClientSocket(), msg.c_str(), msg.size(), 0);
			}
			else
				send(clientsList[i]->getClientSocket(), msg.c_str(), msg.size(), 0);
		}
		else if (sendTo == NOT_ALL_CHANNEL)
		{
			if (cmd.client->getClientSocket() != clientsList[i]->getClientSocket())
				send(clientsList[i]->getClientSocket(), msg.c_str(), msg.size(), 0);
		}
	}	
}

void	Server::sendMsgToServer(std::string msg)
{
	for (size_t i = 1; i < _fds.size(); i++)
		send(_fds[i].fd, msg.c_str(), msg.size(), 0);
}

void	Server::sendMsgToOneClient(std::string msg, t_data &cmd)
{
	Client *tmp = this->getClient(cmd.destUser);
	if (tmp)
		send(tmp->getClientSocket(), msg.c_str(), msg.size(), 0);
}
