
#include "../include/Server.hpp"

int Server::kickCommand(std::string str,Client &client)
{
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() != 3)
		return (ERR_NEEDMOREPARAMS);
	Channel *tmp = getChannel(arr[2]);
	if(!tmp)
		return(ERR_NOSUCHCHANNEL);
	if(!tmp->isOperator(client.getNickName()))
		return(ERR_CHANOPRIVSNEEDED);
	Client *clientTmp = tmp->getClients(client.getNickName());
	if(!clientTmp)
		return(ERR_NOTONCHANNEL);
	std::vector<std::string> split_users = split(arr[3].c_str() + 1,',');
	std::string reason;
	if(arr[4].c_str())
	{
		for(size_t i = 4; i < arr.size(); i++)
		{
			reason += arr[i];
			if (i != arr.size() - 1)
				reason += ' ';
		}
	}
	for(size_t i = 0;i < split_users.size(); i++)
	{
		clientTmp = tmp->getClients(split_users[i]);
		if(!clientTmp)
			std::cout << "ERR_USERNOTINCHANNEL\n";
		else
		{
			if(reason.c_str())
				std::cout << reason << std::endl;
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