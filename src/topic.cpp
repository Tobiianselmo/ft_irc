#include "../include/Server.hpp"

int Server::topicCommand(std::string str,Client &client,t_data &cmd)
{
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() != 2)
		return (ERR_NEEDMOREPARAMS);
	cmd.channel = getChannel(cmd.channelName);
	if(!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL,cmd);
		return(ERR_NOSUCHCHANNEL);
	}
	if(!arr[2].c_str())
	{
		if(cmd.channel->hasTopic())
			return(RPL_TOPIC);
		else
			return(RPL_NOTOPIC);
	}
	if(!cmd.channel->isClient(cmd.client->getNickName()))
	{
		this->createResponse(ERR_NOTONCHANNEL,cmd);
		return(ERR_NOTONCHANNEL);
	}
	std::string topic;
	topic = join(arr.begin() + 2, " ", arr.size() - 2);
	if(!cmd.channel->isOperator(client.getNickName()))
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED,cmd);
		return(ERR_CHANOPRIVSNEEDED);
	}
	else
	{
		cmd.channel->setTopic(topic,true);
		return(RPL_TOPICWHOTIME);
	}
	return(0);
}