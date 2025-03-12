#include "../include/Server.hpp"

int Server::topicCommand(std::string str,Client &client)
{
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() != 2)
		return (ERR_NEEDMOREPARAMS);
	Channel *tmp = getChannel(arr[1].c_str() + 1);
	if(!tmp)
		return(ERR_NOSUCHCHANNEL);
	if(!arr[2].c_str())
	{
		if(tmp->isTopic())
			return(RPL_TOPIC);
		else
			return(RPL_NOTOPIC);
	}
	Client *clientTmp = tmp->getClients(client.getNickName());
	if(!clientTmp)
		return(ERR_NOTONCHANNEL);
	std::string topic;
	topic = join(arr.begin() + 2, " ", arr.size() - 2);
	if(tmp->isOperator(client.getNickName()))
	{
		tmp->setTopic(topic,true);
		return(RPL_TOPICWHOTIME);
	}
	else
		return(ERR_CHANOPRIVSNEEDED);
	return(0);
}