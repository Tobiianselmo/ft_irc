
#include "../include/Server.hpp"

int Server::kickCommand(std::string str, Client &client, t_data &cmd)
{
	std::string msg;
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() < 3)
		return (ERR_NEEDMOREPARAMS);
	cmd.channelName = arr[1];
	Channel *tmp = getChannel(cmd.channelName);
	if(!tmp)
	{
		this->createResponse(ERR_NOSUCHCHANNEL,cmd);
		return(ERR_NOSUCHCHANNEL);
	}
	if(!tmp->isOperator(client.getNickName()))
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED,cmd);
		return(ERR_CHANOPRIVSNEEDED);
	}
	Client *clientTmp = tmp->getClient(client.getNickName());
	if(!clientTmp)
	{
		this->createResponse(ERR_NOTONCHANNEL,cmd);
		return(ERR_NOTONCHANNEL);
	}
	std::vector<std::string> split_users = split(arr[2].c_str(),',');
	std::cout << split_users.size() << std::endl;
	for(size_t i = 0;i < split_users.size(); i++)
	{
		clientTmp = tmp->getClient(split_users[i]);
		if(!clientTmp)
		{
			cmd.destUser = split_users[i];
			this->createResponse(ERR_USERNOTINCHANNEL,cmd);
		}
		else
		{
			sendMsgToChannel(tmp, ":" + client.getNickName() + " " + str + "\r\n");
			tmp->deleteClient(*clientTmp);
		}
	}
	return(0);
}
