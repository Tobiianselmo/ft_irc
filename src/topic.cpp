#include "../include/Server.hpp"
void Server::topicCommand(std::string str, Client &client, t_data &cmd)
{
	cmd.cmdType = "TOPIC";
	std::vector<std::string> arr = split(str, ' ');

	if (arr.size() != 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	Channel *tmp = getChannel(arr[1].c_str() + 1);
	if (!tmp)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd);
		return ;
	}
	if (!arr[2].c_str())
	{
		if (tmp->hasTopic())
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

	Client *clientTmp = tmp->getClient(client.getNickName());
	if (!clientTmp)
	{
		this->createResponse(ERR_NOTONCHANNEL, cmd);
		return ;
	}

	std::string topic = join(arr.begin() + 2, " ", arr.size() - 2);
	if (tmp->isOperator(client.getNickName()))
	{
		tmp->setTopic(topic, true);
		this->createResponse(RPL_TOPICWHOTIME, cmd);
	}
	else
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd);
}