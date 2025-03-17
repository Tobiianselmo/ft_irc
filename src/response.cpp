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

void	Channel::sendModes(t_data &cmd)
{
	std::string tmp;

	if (this->getInvite() == false)
		tmp = ": INVITE MODE : Not set.\r\n";
	else
		tmp = ": INVITE MODE : Set.\r\n";
	send(cmd.client->getClientSocket(), tmp.c_str(), tmp.size(), 0);
	if (this->hasPassword() == false)
		tmp = ": KEY MODE : Not set.\r\n";
	else
		tmp = ": KEY MODE : Set.\r\n";
	send(cmd.client->getClientSocket(), tmp.c_str(), tmp.size(), 0);
	if (this->hasTopic() == false)
		tmp = ": TOPIC MODE : Not set.\r\n";
	else
		tmp = ": TOPIC MODE : Set.\r\n";
	send(cmd.client->getClientSocket(), tmp.c_str(), tmp.size(), 0);
}

void Server::createResponse(int err, t_data &cmd)
{
	std::string prefix = ":ft_irc " + intToString(err) + " ";
	std::string response;
	
	if (err == ERR_UNKNOWNCOMMAND)  // finished
		response = prefix + cmd.client->getNickName() + " :Unknown command\r\n";
	else if (err == ERR_BADCHANNELKEY)  // finished
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " :Cannot join channel (+k)\r\n";
	else if (err == ERR_BADCHANMASK)  // finished
		response = prefix + cmd.channelName + " :Bad channel mask\r\n";
	else if (err == ERR_NEEDMOREPARAMS)  // finished
		response = ":" + cmd.client->getNickName() + " " + cmd.cmdType + " :Not enough parameters\r\n";
	else if (err == RPL_JOIN)  // finished
		response = ":" + cmd.client->getNickName() + " JOIN " + cmd.channelName + "\r\n";
	else if (err == RPL_NAMREPLY)  // finished
		response = rpl_namreply(this, cmd, intToString(err));
	else if (err == RPL_ENDOFNAMES) // finished
		response = ":" + this->getHostName() + " " + intToString(err) + " " + cmd.client->getNickName() + " " + cmd.channelName + " :End of /NAMES list\r\n";
	else if (err == ERR_NICKNAMEINUSE)
		response = prefix + " :Nickname is already in use\r\n";
		// else if (err == ERR_USERNOTINCHANNEL)
	// 	response = cmd.client->getNickName() + " " + cmd.destUser + " " + cmd.channelName + " :They aren't on that channel\r\r";
	// else if (err == RPL_LEAVE)//check
	// 	response = ":" + this->getHostName() + " LEAVE : " + cmd.client->getNickName() + " has left channel " + cmd.channelName + ".\r\n"; 
		// else if (err == -1)
	// 	response = ": MODE 
	// else if (err == ERR_NOSUCHCHANNEL)
	// 	response = cmd.client->getNickName() + " " + cmd.channelName + " :No such channel\r\n";
	// else if (err == ERR_CHANOPRIVSNEEDED)
	// 	response = cmd.client->getNickName() + " " + cmd.channelName + " :You're not channel operator\r\n";
	// else if (err == ERR_NOTONCHANNEL)
	// 	response = cmd.client->getNickName() + " " + cmd.channelName + " :You're not on that channel\r\n";
	else
		response = "";
	send(cmd.client->getClientSocket(), response.c_str(), response.size(), 0);
}