#include "../../include/Server.hpp"

void Server::kickCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "KICK";
	std::vector<std::string> arr = split(line,' ');
	if (arr.size() < 3)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}
	cmd.channelName = arr[1];
	cmd.channel = getChannel(cmd.channelName);
	if (!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}
	if (!cmd.channel->isClient(cmd.client->getNickName()))
	{
		this->createResponse(ERR_NOTONCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}
	if (!cmd.channel->isOperator(client.getNickName()))
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd, ONLY_CLIENT);
		return ;
	}
	Client *clientTmp;
	std::vector<std::string> splitUsers = split(arr[2].c_str(), ',');
	for (size_t i = 0; i < splitUsers.size(); i++)
	{
		clientTmp = cmd.channel->getClient(splitUsers[i]);
		if (!clientTmp)
		{
			cmd.destUser = splitUsers[i];
			this->createResponse(ERR_USERNOTINCHANNEL, cmd, ONLY_CLIENT);
		}
		else
		{
			cmd.msg = cmd.cmdType + " " + cmd.channelName + " " + splitUsers[i];
			if (arr.size() > 3)
				cmd.msg += " " + join(arr.begin() + 3 , " ", arr.size() - 3);
			this->createResponse(RPL_KICK, cmd, ALL_CHANNEL);
			cmd.channel->deleteClient(clientTmp);
		}
	}
	return ;
}
