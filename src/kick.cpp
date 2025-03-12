
#include "../include/Server.hpp"

int Server::kickCommand(std::string str,Client &client)
{
	std::string msg;
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() <= 3)
		return (ERR_NEEDMOREPARAMS);
	Channel *tmp = getChannel(arr[2]);
	if(!tmp)
	{
		msg = makeString(arr[2],client.getUserName(),"No such channel\r\n",ERR_NOSUCHCHANNEL,".");
		sendClient(client, msg.c_str());
		return(ERR_NOSUCHCHANNEL);
	}
	if(!tmp->isOperator(client.getNickName()))
	{
		msg = makeString(arr[2],client.getUserName(),":You're not channel operator\r\n",ERR_CHANOPRIVSNEEDED,".");
		sendClient(client, msg.c_str());
		return(ERR_CHANOPRIVSNEEDED);
	}
	Client *clientTmp = tmp->getClients(client.getNickName());
	if(!clientTmp)
	{
		msg = makeString(arr[2],client.getUserName(),":You're not on that channel\r\n",ERR_NOTONCHANNEL,".");
		sendClient(client, msg.c_str());
		return(ERR_NOTONCHANNEL);
	}
	std::vector<std::string> split_users = split(arr[3].c_str() + 1,',');
	std::string reason;
	if(arr[4].c_str())
		reason = join(arr.begin() + 4, " ", arr.size() - 4);
	for(size_t i = 0;i < split_users.size(); i++)
	{
		clientTmp = tmp->getClients(split_users[i]);
		if(!clientTmp)
		{
			msg = makeString(arr[2],client.getUserName(),":They aren't on that channel\r\n",ERR_USERNOTINCHANNEL,client.getNickName());
			sendClient(client,msg.c_str());
		}
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