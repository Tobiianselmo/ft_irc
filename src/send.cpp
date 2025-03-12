#include "../include/Server.hpp"

void	sendClient(Client &client,const char *str)
{
	send(client.getClientSocket(), str, std::strlen(str), 0);
}

void	sendMsgToChannel(Channel *channel, std::string msg)
{
	std::vector<Client> clientsList = channel->getArrClients();
	for (size_t i = 0; i < clientsList.size(); i++)
	{
		send(clientsList[i].getClientSocket(), msg.c_str(), msg.size(), 0);
	}
}