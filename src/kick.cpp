
#include "../include/Server.hpp"

void Server::kickCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "KICK";
	std::string msg;
	std::vector<std::string> arr = split(line,' ');
	if (arr.size() < 3)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	cmd.channelName = arr[1];
	Channel *tmp = getChannel(cmd.channelName);
	if (!tmp)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd);
		return ;
	}
	if (!tmp->isOperator(client.getNickName()))
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd);
		return ;
	}
	Client *clientTmp = tmp->getClient(client.getNickName());
	if (!clientTmp)
	{
		this->createResponse(ERR_NOTONCHANNEL, cmd);
		return ;
	}

	std::vector<std::string> splitUsers = split(arr[2].c_str(), ',');

	for (size_t i = 0; i < splitUsers.size(); i++)
	{
		clientTmp = tmp->getClient(splitUsers[i]);
		if (!clientTmp)
		{
			cmd.destUser = splitUsers[i];
			this->createResponse(ERR_USERNOTINCHANNEL, cmd);
		}
		else
		{
			sendMsgToChannel(tmp, ":" + client.getNickName() + " " + line + "\r\n");
			tmp->deleteClient(*clientTmp);
		}
	}
	return ;
}
