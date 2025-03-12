
#include "../include/Server.hpp"

int Server::kickCommand(std::string str,Client &client)
{
	std::string msg;
	std::vector<std::string> arr = split(str,' ');
	if (arr.size() < 3)
		return (ERR_NEEDMOREPARAMS);
	Channel *tmp = getChannel(arr[1]);
	if(!tmp)
	{
		msg = makeString(arr[1],client.getUserName(),"No such channel\r\n",ERR_NOSUCHCHANNEL,".");
		sendClient(client, msg.c_str());
		return(ERR_NOSUCHCHANNEL);
	}
	if(!tmp->isOperator(client.getNickName()))
	{
		msg = makeString(arr[1],client.getUserName(),":You're not channel operator\r\n",ERR_CHANOPRIVSNEEDED,".");
		sendClient(client, msg.c_str());
		return(ERR_CHANOPRIVSNEEDED);
	}
	Client *clientTmp = tmp->getClient(client.getNickName());
	if(!clientTmp)
	{
		msg = makeString(arr[1],client.getUserName(),":You're not on that channel\r\n",ERR_NOTONCHANNEL,".");
		sendClient(client, msg.c_str());
		return(ERR_NOTONCHANNEL);
	}
	std::vector<std::string> split_users = split(arr[2].c_str(),',');
	// std::string reason;
	// if(arr[3].c_str())
	// 	reason = join(arr.begin() + 3, " ", arr.size() - 3);
	std::cout << split_users.size() << std::endl;
	for(size_t i = 0;i < split_users.size(); i++)
	{
		clientTmp = tmp->getClient(split_users[i]);
		if(!clientTmp)
		{
			msg = makeString(arr[1],client.getUserName(),":They aren't on that channel\r\n",ERR_USERNOTINCHANNEL,client.getNickName());
			sendClient(client,msg.c_str());
		}
		else
		{
			// if(reason.c_str())
			// 	std::cout << reason << std::endl;
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