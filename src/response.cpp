#include "../include/Server.hpp"

std::string Server::rpl_namreply(Client &client, Channel *channel)
{
	std::string symbol = "=";
	std::string response;
	std::string clientsList;
	std::vector<Client> clients;
	// std::cout << &channel << std::endl;
	clients = channel->getArrClients();
	
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (channel->isOperator(clients[i].getNickName()) == true)
			clientsList += "@" + clients[i].getNickName() + " ";
		else
			clientsList += clients[i].getNickName() + " ";
	}
	response += ":" + this->getHostName() + " 353 " + client.getNickName() + " " + symbol + " " + channel->getName() + " :" + clientsList + "\r\n";
	// std::cout << response << std::endl;
	return response;
}

std::string Server::rpl_endofnames(Client &client, Channel *channel)
{
	std::string response;

	response += ":" + this->getHostName() + " 366 " + client.getNickName() + " " + channel->getName() + " :" + channel->getTopic() + "\r\n";
	return response;
}

void Server::createResponse(int err, Client &client, const std::string &channelName)
{
	std::string clientNickName = client.getNickName();
	std::string response;
	std::string second;

	switch (err)
	{
		case RPL_NAMREPLY:
		{
			// std::cout << "llega" << std::endl;
			response = this->rpl_namreply(client, this->getChannel(channelName));
			send(client.getClientSocket(), response.c_str(), response.size(), 0);
			// std::cout << response << std::endl;
			second = this->rpl_endofnames(client, this->getChannel(channelName));
			send(client.getClientSocket(), second.c_str(), second.size(), 0);
			break ;
		}	
		default:
			std::cout << "Ningun error correcto\n";
	}
}