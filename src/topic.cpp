#include "../include/Server.hpp"
void Server::topicCommand(std::string str, Client &client, t_data &cmd)
{
	cmd.cmdType = "TOPIC";
	std::vector<std::string> arr = split(str, ' ');

	if (arr.size() < 2)
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
	if (!arr[2].c_str())
	{
		if (cmd.channel->hasTopic())
		{
			this->createResponse(RPL_TOPIC, cmd);
			return ;
		}
		else
		{
			this->createResponse(RPL_NOTOPIC, cmd);
			return ;
		}
	}
	if(!cmd.channel->isClient(cmd.client->getNickName()))
	{
		this->createResponse(ERR_NOTONCHANNEL,cmd);
		return;
	}
	std::string topic = join(arr.begin() + 2, " ", arr.size() - 2);
	if (cmd.channel->isOperator(client.getNickName()))
	{
		cmd.channel->setTopic(topic, true);
		this->createResponse(RPL_TOPICWHOTIME, cmd);
	}
	else
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd);
}