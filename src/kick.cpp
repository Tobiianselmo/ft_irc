
#include "../include/Server.hpp"

void Server::kickCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "KICK";
	std::vector<std::string> arr = split(line,' ');
	if (arr.size() < 3)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	cmd.channelName = arr[1];
	cmd.channel = getChannel(cmd.channelName);
	if(!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL,cmd);
		return;
	}
	if(!cmd.channel->isOperator(client.getNickName()))
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED,cmd);
		return;
	}
	if(!cmd.channel->isClient(cmd.client->getNickName()))
	{
		this->createResponse(ERR_NOTONCHANNEL,cmd);
		return;
	}
	Client *clientTmp;
	std::vector<std::string> split_users = split(arr[2].c_str(),',');
	for(size_t i = 0;i < split_users.size(); i++)
	{
		clientTmp = cmd.channel->getClient(split_users[i]);
		if(!clientTmp)
		{
			cmd.destUser = split_users[i];
			this->createResponse(ERR_USERNOTINCHANNEL,cmd);
		}
		else
		{
			cmd.msg = cmd.cmdType + " " + cmd.channelName + " " + split_users[i];
			if (arr[3].c_str())
				cmd.msg += " " + join(arr.begin() + 3 , " ",arr.size() - 3);
			sendMsgToChannel(cmd.channel, ":" + client.getNickName() + " " + cmd.msg + "\r\n");
			cmd.channel->deleteClient(*clientTmp);
		}
	}
	return ;
}
