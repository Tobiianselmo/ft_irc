
#include "../include/Server.hpp"

void	Server::inviteCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "INVITE";
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() != 3)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}

	Channel	*tmp = getChannel(parameters[2]);
	if (!tmp)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}
	if (tmp->isOperator(client.getNickName()) == false)
	{
		this->createResponse(ERR_NOOPERHOST, cmd, ONLY_CLIENT);
		return ;
	}

	Client	*clientTmp = this->getClient(parameters[1]);
	if (!clientTmp)
	{
		this->createResponse(ERR_NOSUCHNICK, cmd, ONLY_CLIENT);
		return ;
	}
	tmp->addInvite(clientTmp);
	tmp->printInvited();
	return ;
}
