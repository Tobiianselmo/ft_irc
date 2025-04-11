
#include "../../include/Server.hpp"

void	Server::partCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "PART";
	cmd.msg = line;
	cmd.client = &client;
	std::vector<std::string>	parameters = split(line, ' ');

	if (parameters.size() < 3)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}
	cmd.channelName = parameters[1];
	cmd.channel = this->getChannel(parameters[1]);
	if (!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}
	if (!cmd.channel->isClient(client))
	{
		this->createResponse(ERR_NOTONCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}

	if (cmd.channel->isOperator(client.getNickName()) && cmd.channel->getOperatorsSize() == 1 && cmd.channel->getUserSize() > 1)
	{
		std::vector<Client *> clientAux = cmd.channel->getArrClients();

		for (int cont = 0; cont < (int)clientAux.size() ; cont++)
		{
			if (clientAux[cont]->getNickName() != client.getNickName())
			{
				cmd.channel->addOperator(clientAux[cont]);
				this->createResponse(RPL_NAMREPLY, cmd, NOT_ALL_CHANNEL);
				this->createResponse(RPL_ENDOFNAMES, cmd, NOT_ALL_CHANNEL);
				break ;
			}
		}
	}
	this->createResponse(RPL_PART, cmd, ALL_CHANNEL);
	cmd.channel->deleteClient(&client);
}
