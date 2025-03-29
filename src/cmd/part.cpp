
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

	bool opChange = false;

	if (cmd.channel->isOperator(client.getNickName()) && cmd.channel->getOperatorsSize() == 1)
	{
		std::vector<Client *>	clientAux = cmd.channel->getArrClients();
		for (int i = 0; i < (int)clientAux.size(); i++)
		{
			if (clientAux[i]->getNickName() != client.getNickName())
			{
				cmd.channel->addOperator(clientAux[i]);
				opChange = true;
				cmd.destUser = clientAux[i]->getNickName();
				break ;
			}
		}
	}
	this->createResponse(RPL_PART, cmd, ALL_CHANNEL);
	cmd.channel->deleteClient(&client);
	if (opChange)
		this->createResponse(RPL_OPERATOR, cmd, ALL_CHANNEL);
}
