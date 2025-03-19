
#include "../include/Server.hpp"

int Server::kickCommand(std::string str, Client &client, t_data &cmd)
{
	std::string msg;
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() < 3)
		return (ERR_NEEDMOREPARAMS);
	cmd.channelName = arr[1];
	cmd.channel = getChannel(cmd.channelName);
	if(!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL,cmd);
		return(ERR_NOSUCHCHANNEL);
	}
	if(!cmd.channel->isOperator(client.getNickName()))
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED,cmd);
		return(ERR_CHANOPRIVSNEEDED);
	}
	if(!cmd.channel->isClient(cmd.client->getNickName()))
	{
		this->createResponse(ERR_NOTONCHANNEL,cmd);
		return(ERR_NOTONCHANNEL);
	}
	Client *clientTmp;
	std::vector<std::string> split_users = split(arr[2].c_str(),',');
	std::cout << split_users.size() << std::endl;
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
			sendMsgToChannel(cmd.channel, ":" + client.getNickName() + " " + str + "\r\n");
			cmd.channel->deleteClient(*clientTmp);
		}
	}
	return(0);
}
