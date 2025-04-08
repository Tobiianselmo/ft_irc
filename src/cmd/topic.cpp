#include "../../include/Server.hpp"

void Server::topicCommand(std::string str, Client &client, t_data &cmd)
{
	cmd.cmdType = "TOPIC";
	std::vector<std::string> arr = split(str, ' ');
	if (arr.size() < 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}
	cmd.channelName = arr[1];
	cmd.channel = getChannel(cmd.channelName);
	if (!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
		return;
	}
	if (!arr[2].c_str())
	{
		if (cmd.channel->hasTopic())
		{
			this->createResponse(RPL_TOPIC, cmd, ONLY_CLIENT);
			this->createResponse(RPL_TOPICWHOTIME, cmd, ALL_CHANNEL);
			return ;
		}
		else
		{
			this->createResponse(RPL_NOTOPIC, cmd, ONLY_CLIENT);
			return ;
		}
	}
	if (!cmd.channel->isClient(cmd.client->getNickName()))
	{
		this->createResponse(ERR_NOTONCHANNEL,cmd, ONLY_CLIENT);
		return ;
	}
	if (cmd.channel->getAllowedTopic() == true)
	{
		if (cmd.channel->isOperator(client.getNickName()))
		{
			if (arr[2].size() > 1)
			{
				arr[2] = arr[2].c_str() + 1;
				std::string topic = join(arr.begin() + 2, " ", arr.size() - 2);
				cmd.channel->setTopic(topic, true, cmd.client->getNickName());
				this->createResponse(RPL_TOPIC, cmd, ALL_CHANNEL);
				this->createResponse(RPL_TOPICWHOTIME, cmd, ALL_CHANNEL);
			}
			else
			{
				cmd.channel->setTopic("", false, cmd.client->getNickName());
				this->createResponse(RPL_REMOVETOPIC, cmd, ALL_CHANNEL);
				this->createResponse(RPL_TOPIC, cmd, ALL_CHANNEL);
			}
		}
		else
			this->createResponse(ERR_CHANOPRIVSNEEDED, cmd, ONLY_CLIENT);
	}
	else
	{
		if (arr[2].size() > 1)
		{
			arr[2] = arr[2].c_str() + 1;
			std::string topic = join(arr.begin() + 2, " ", arr.size() - 2);
			cmd.channel->setTopic(topic, true, cmd.client->getNickName());
			this->createResponse(RPL_TOPIC, cmd, ALL_CHANNEL);
			this->createResponse(RPL_TOPICWHOTIME, cmd, ALL_CHANNEL);
		}
		else
		{
			cmd.channel->setTopic("", false, cmd.client->getNickName());
			this->createResponse(RPL_REMOVETOPIC, cmd, ALL_CHANNEL);
			this->createResponse(RPL_TOPIC, cmd, ALL_CHANNEL);
		}
	}
}