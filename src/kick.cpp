
#include "../include/Server.hpp"

int Server::kickCommand(std::string str,Client &client)
{
	t_data data;
	std::string msg;
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() < 3)
		return (ERR_NEEDMOREPARAMS);
	data.Channel = arr[1];
	Channel *tmp = getChannel(data.Channel);
	if(!tmp)
	{
		this->createResponse(ERR_NOSUCHCHANNEL,client,&data);
		return(ERR_NOSUCHCHANNEL);
	}
	if(!tmp->isOperator(client.getNickName()))
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED,client,&data);
		return(ERR_CHANOPRIVSNEEDED);
	}
	Client *clientTmp = tmp->getClient(client.getNickName());
	if(!clientTmp)
	{
		this->createResponse(ERR_NOTONCHANNEL,client,&data);
		return(ERR_NOTONCHANNEL);
	}
	std::vector<std::string> split_users = split(arr[2].c_str(),',');
	std::cout << split_users.size() << std::endl;
	for(size_t i = 0;i < split_users.size(); i++)
	{
		clientTmp = tmp->getClient(split_users[i]);
		if(!clientTmp)
		{
			data.user = split_users[i];
			this->createResponse(ERR_USERNOTINCHANNEL,client,&data);
		}
		else
		{
			sendMsgToChannel(tmp, ":" + client.getNickName() + " " + str + "\r\n");
			tmp->deleteClient(*clientTmp);
		}
	}
	return(0);
}

void	Channel::deleteClient(const Client &client)
{
	int cont;

	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickName() == client.getNickName())
			cont = i;
	}
	this->_clients.erase(this->_clients.begin() + cont);
}