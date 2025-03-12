#include "../include/Server.hpp"

std::string rpl_namreply(Server *server, Client &client, Channel *channel)
{
	// std::string symbol = "=";
	std::string response;
	std::string clientsList;
	std::vector<Client> clients;
	clients = channel->getArrClients();
	
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (channel->isOperator(clients[i].getNickName()) == true)
			clientsList += "@" + clients[i].getNickName() + " ";
		else
			clientsList += clients[i].getNickName() + " ";
	}
	response = ":" + server->getHostName() + " 353 " + client.getNickName() + " = " + channel->getName() + " :" + clientsList + "\r\n";
	return response;
}

std::string rpl_endofnames(Server *server, Client &client, Channel *channel)
{
	std::string response;

	response = ":" + server->getHostName() + " 366 " + client.getNickName() + " " + channel->getName() + " :End of /NAMES list\r\n";
	return response;
}

void Server::createResponse(int err, Client &client, const std::string &channelName)
{
	std::string clientNickName = client.getNickName();
	std::string response;

	switch (err)
	{
		case ERR_NEEDMOREPARAMS:
			response = "";
			break ;
		case RPL_JOIN:
			response = ":" + client.getNickName() + " JOIN " + channelName + "\r\n";
			break ;
		case RPL_NAMREPLY:
			response = rpl_namreply(this, client, this->getChannel(channelName));
			break ;
		case RPL_ENDOFNAMES:
			response = rpl_endofnames(this, client, this->getChannel(channelName));
			break ;
		default:
			response = "";
	}
	send(client.getClientSocket(), response.c_str(), response.size(), 0);
}