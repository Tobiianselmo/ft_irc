#include "../include/Server.hpp"

std::string rpl_namreply(Server *server, t_data &cmd, std::string err)
{
	std::string response;
	std::string clientsList;
	std::vector<Client *> clients;
	clients = cmd.channel->getArrClients();

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (cmd.channel->isOperator(clients[i]->getNickName()) == true)
			clientsList += "@" + clients[i]->getNickName() + " ";
		else
			clientsList += clients[i]->getNickName() + " ";
	}
	response = ":" + server->getHostName() + " " + err + " " + cmd.client->getNickName() + " = " + cmd.channel->getName() + " :" + clientsList + "\r\n";
	return response;
}

void	Channel::sendModes(t_data &cmd, int num)
{
	std::string tmp;
	std::string	prefix = ":ft_irc " + intToString(num) + " " + cmd.client->getNickName() + " " + cmd.channel->getName() + " :+";

	if (this->getInvite() == true)
		tmp = "i";
	if (this->hasPassword() == true)
	{
		if (tmp.size() > 0)	//consultar
			tmp += "k";
		else
			tmp = "k";
	}
	if (this->getAllowedTopic() == true)
	{
		if (tmp.size() > 0)	//consultar
			tmp += "t";
		else
			tmp = "t";
	}
	if (this->hasLimit() == true)
	{
		if (tmp.size() > 0)
			tmp += "l";
		else
			tmp = "l";
	}
	if (tmp.size() > 0)	
		prefix += tmp;
	prefix += "\r\n";
	send(cmd.client->getClientSocket(), prefix.c_str(), prefix.size(), 0);
}

void Server::createResponse(int err, t_data &cmd, int sendTo)
{
	std::string prefix = ":ft_irc " + intToString(err) + " ";

	std::string response;

	//General responses
	if (err == ERR_UNKNOWNCOMMAND)  // finished
		response = prefix + cmd.client->getNickName() + " :Unknown command\r\n";
	else if (err == ERR_NEEDMOREPARAMS)  // finished
		response = prefix + cmd.client->getNickName() + " " + cmd.cmdType + " :Not enough parameters\r\n";
	
	//Invite responses
	else if (err == RPL_INVITING)
		response = ":" + cmd.client->getNickName() + " " + cmd.msg + "\r\n";
	else if (err == ERR_USERONCHANNEL)
		response = prefix + cmd.client->getNickName() + " " + cmd.destUser + " " + cmd.channelName + " :Is already on channel\r\n";
	
	//Quit responses
	else if (err == RPL_QUIT)
		response = ":" + cmd.client->getNickName() + " " + cmd.msg + "\r\n";

	//Privmsg responses
	else if (err == RPL_PRIVMSGSUCCESS)
		response = ":" + cmd.client->getNickName() + "!" + this->getHostName() + " " + cmd.cmdType + " " + cmd.destUser + " " + cmd.privMessage + "\r\n"; 
	else if (err == ERR_NOTEXTTOSEND)
		response = prefix + cmd.client->getNickName() + " :No text to send\r\n";
	else if (err == ERR_CANNOTSENDTOCHAN)
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " :Cannot send to channel\r\n";
	else if (err == ERR_NOSUCHNICK)
		response = prefix + cmd.client->getNickName() + " " + cmd.destUser + " :No such nick/channel\r\n";

	//Join responses
	else if (err == RPL_JOIN)  // finished
		response = ":" + cmd.client->getNickName() + " JOIN " + cmd.channelName + "\r\n";
	else if (err == ERR_BADCHANMASK)  // finished
		response = prefix + cmd.channelName + " :Bad channel mask\r\n";
	else if (err == RPL_NAMREPLY)
		response = rpl_namreply(this, cmd, intToString(err));
	else if (err == RPL_ENDOFNAMES)
		response = ":" + this->getHostName() + " " + intToString(err) + " " + cmd.client->getNickName() + " " + cmd.channelName + " :End of /NAMES list\r\n";
	else if (err == ERR_BADCHANNELKEY)  // finished
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " :Cannot join channel (+k)\r\n";
	else if (err == ERR_INVITEONLYCHAN)
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " :Cannot join channel (+i)\r\n"; 

	//Topic responses
	else if (err == RPL_TOPIC)
		response = prefix + cmd.client->getNickName() + " " + cmd.channel->getName() + " :" + cmd.channel->getTopic() + "\r\n";
	else if (err == RPL_NOTOPIC)
		response = prefix + cmd.client->getNickName() + " " + cmd.channel->getName() + " :No topic is set\r\n";
	else if (err == ERR_NOSUCHCHANNEL)
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " :No such channel\r\n";
	else if (err == RPL_TOPICWHOTIME)
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " " + cmd.channel->getTopicCreate() + " " + cmd.channel->getNowTime() + "\r\n";
	else if (err == RPL_REMOVETOPIC)
		response = prefix + "test " + cmd.channelName + " :Clearing the topic on " + cmd.channelName + "\r\n";

	//Kick responses
	else if (err == RPL_KICK)
		response = ":" + cmd.client->getNickName() + " " + cmd.msg + "\r\n";
	else if(err ==  ERR_CHANOPRIVSNEEDED)
		response = prefix + cmd.client->getNickName() + " " + cmd.channelName + " :You're not channel operator\r\n";
	else if(err ==  ERR_NOTONCHANNEL)
		response = cmd.client->getNickName() + " " + cmd.channelName + " :You're not on that channel\r\n";
	else if(err ==  ERR_USERNOTINCHANNEL)
		response = prefix + " " + cmd.client->getNickName() + " " + cmd.destUser + " " + cmd.channelName + " :They aren't on that channel\r\n";

	//Nick respones
	else if (err == RPL_NICKSUCCESS)
		response = ":" + cmd.client->getNickName() + "!" + this->getHostName() + " " + cmd.authMsg + "\r\n";
	else if (err == ERR_ERRONEUSNICKNAME)
		response = prefix + " :Erroneus nickname\r\n";
	else if (err == ERR_NICKNAMEINUSE)
		response = prefix + " :Nickname is already in use\r\n";
	else if (err == ERR_NOTCORRECTNICK)
		response = prefix + " :Introduce the correct nickname to continue the authentication process\r\n";
	
	//Pass responses
	else if (err == ERR_PASSWDMISMATCH)
		response = prefix + "(client) :Password incorrect\r\n";
	else if (err == ERR_NOTCORRECTPASS)
		response = prefix + "(client) :Introduce the correct password to continue the authentication process\r\n";
	else if (err == ERR_ALREADYREGISTERED)
		response = prefix + "(client) :You may not reregister\r\n";

	//Mode responses
	else if (err == ERR_INVALIDCHAR)
		response = prefix + cmd.client->getNickName() + " :Invalid char in MODE command\r\n";
	else if (err == ERR_INVALIDMODEPARAM)
		response = prefix + cmd.client->getNickName() + " " + cmd.channel->getName() + cmd.description;

	else if (err == ERR_KEYSET)
		response = prefix + cmd.client->getNickName() + " :Channel key already set\r\n";
	else if (err == ERR_LIMITSET)
		response = prefix + cmd.client->getNickName() + " :Channel user limit already set\r\n";
	else if (err == ERR_INVALIDLIMIT)
		response = prefix + cmd.client->getNickName() + " :Can't set a channel limit higher than " + intToString(cmd.channel->getUserSize()) + "\r\n";
	else if (err == ERR_UNKNOWNMODE)
		response = prefix + cmd.client->getNickName() + " :Unknown mode\r\n";
	else if (err == RPL_OPERATOR)
		response = ":" + cmd.client->getNickName() + "!" + this->getHostName() + " MODE " + cmd.channel->getName() + " +o :" + cmd.destUser + "\r\n";

	else if (err == RPL_PART)
		response = ":" + cmd.client->getNickName() + "!" + this->getHostName() + " " + cmd.msg + "\r\n";

	else
		response = "";
	if (sendTo == ALL_CHANNEL || sendTo == ONLY_OPERATORS || sendTo == NOT_ALL_CHANNEL)
		sendMsgToChannel(response, sendTo, cmd);
	else if (sendTo == ALL_CLIENTS)
		sendMsgToServer(response);
	else if (sendTo == ONE_CLIENT)
		sendMsgToOneClient(response, cmd);
	else
		send(cmd.client->getClientSocket(), response.c_str(), response.size(), 0);
}
