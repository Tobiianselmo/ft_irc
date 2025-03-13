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

void Server::createResponse(int err, Client &client,t_data *data)
{
	std::string clientNickName = client.getNickName();
	std::string response;

	switch (err)
	{
		case ERR_PASSWDMISMATCH:
			response = client.getNickName() + " " + ":Password incorrect\r\n";
			break;
		case ERR_NEEDMOREPARAMS:
			response = "";
			break ;
		case RPL_JOIN:
			response = ":" + client.getNickName() + " JOIN " + data->Channel + "\r\n";
			break ;
		case RPL_NAMREPLY:
			response = rpl_namreply(this, client, this->getChannel(data->Channel));
			break ;
		case RPL_ENDOFNAMES:
			response = rpl_endofnames(this, client, this->getChannel(data->Channel));
			break ;
		case ERR_NOSUCHCHANNEL:
			response = 	clientNickName + " " + data->Channel + " :No such channel\r\n";
			break;
		case ERR_CHANOPRIVSNEEDED:
			response = clientNickName + " " + data->Channel + " :You're not channel operator\r\n"; ///REVSAR
			break;
		case ERR_NOTONCHANNEL:
			response = clientNickName + " " + data->Channel + " :You're not on that channel\r\n";
			break;
		case ERR_USERNOTINCHANNEL:
			response = ":ft_irc " + intToString(ERR_USERNOTINCHANNEL) + " " + clientNickName + " " + data->user + " " + data->Channel + " :They aren't on that channel\r\n";
			break;
/* 		default:
			response = ""; */
	}
	send(client.getClientSocket(), response.c_str(), response.size(), 0);
}