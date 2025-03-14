#include "../include/Server.hpp"

std::string rpl_namreply(Server *server, t_data &cmd, std::string err)
{
	std::string response;
	std::string clientsList;
	std::vector<Client> clients;
	clients = cmd.channel->getArrClients();

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (cmd.channel->isOperator(clients[i].getNickName()) == true)
			clientsList += "@" + clients[i].getNickName() + " ";
		else
			clientsList += clients[i].getNickName() + " ";
	}
	response = ":" + server->getHostName() + " " + err + " " + cmd.client->getNickName() + " = " + cmd.channel->getName() + " :" + clientsList + "\r\n";
	return response;
}

std::string rpl_endofnames(Server *server, t_data &cmd, std::string err)
{
	std::string response;

	response = ":" + server->getHostName() + " " + err + " " + cmd.client->getNickName() + " " + cmd.channel->getName() + " :End of /NAMES list\r\n";
	return response;
}

void Server::createResponse(int err, t_data &cmd)
{
	std::string prefix = ":ft_irc " + intToString(err) + " ";
	std::string response;

	
	if (err == ERR_UNKNOWNCOMMAND)
		response = prefix + cmd.client->getNickName() + " :Unknown command\r\n";
	else if (err == ERR_BADCHANNELKEY)
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " :Cannot join channel (+k)\r\n";
	else if (err == ERR_BADCHANMASK)
		response = prefix + cmd.channelName + " :Bad channel mask\r\n";
	else if (err == ERR_NEEDMOREPARAMS)
		response = ":" + cmd.client->getNickName() + " " + cmd.cmdType + " :Not enough parameters\r\n";
	else if (err == RPL_JOIN)
		response = ":" + cmd.client->getNickName() + " JOIN " + cmd.channel->getName() + "\r\n";
	else if (err == RPL_NAMREPLY)
		response = rpl_namreply(this, cmd, intToString(err));
	else if (err == RPL_ENDOFNAMES)
		response = rpl_endofnames(this, cmd, intToString(err));
	// case ERR_NOSUCHCHANNEL:
	// 	response = 	cmd.client->getNickName() + " " + cmd.channel->getName() + " :No such channel\r\n";
	// case ERR_CHANOPRIVSNEEDED:
	// 	response = cmd.client->getNickName() + " " + cmd.channel->getName() + " :You're not channel operator\r\n";
	// case ERR_NOTONCHANNEL:
	// 	response = cmd.client->getNickName() + " " + cmd.channel->getName() + " :You're not on that channel\r\n";
	// case ERR_USERNOTINCHANNEL:
	// 	response = cmd.client->getNickName() + " " + cmd.user + cmd.channel->getName() + " :They aren't on that channel\r\r";
	else
		response = "";
	send(cmd.client->getClientSocket(), response.c_str(), response.size(), 0);
}