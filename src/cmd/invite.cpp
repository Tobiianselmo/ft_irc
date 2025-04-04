#include "../../include/Server.hpp"

void	Server::inviteCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "INVITE";
	cmd.msg = commandToUpper(line);
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() != 3)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}

	cmd.channelName = parameters[2];
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

	cmd.destUser = parameters[1];
	Client	*clientTmp = this->getClient(parameters[1]);
	if (!clientTmp)
		this->createResponse(ERR_NOSUCHNICK, cmd, ONLY_CLIENT);
	else
	{
		if (tmp->isClient(clientTmp->getNickName()) == true)
			this->createResponse(ERR_USERONCHANNEL, cmd, ONLY_CLIENT);
		else
		{
			cmd.destUser = clientTmp->getNickName();//tiene sentido tenerlo acá tambien?
			tmp->addInvite(clientTmp);
			this->createResponse(RPL_INVITING, cmd, ONE_CLIENT);
		}
	}
}
