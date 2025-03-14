#include "../include/Server.hpp"

void Server::passCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "PASS";
	if (line.size() < 6)
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
	else if (line.c_str() + 5 != this->getPassword())
		this->createResponse(ERR_PASSWDMISMATCH, cmd);
	else
		client.setCorrectPass(true);
}

void Server::nickCommand(std::vector<std::string> arr, Client &client, t_data &cmd)
{
	cmd.cmdType = "NICK";
	if (arr[0].size() < 6)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	const char *nick = checkNickName(arr[0].c_str() + 5);
	if (nick == NULL)
		this->createResponse(ERR_ERRONEUSNICKNAME, cmd);
	else if (isDuplicated(nick) == true)
		this->createResponse(ERR_NICKNAMEINUSE, cmd);
	if (client.hasCorrectPass() == true)
	{
		client.setNickName(nick);
		client.setAuth(true);
	}
	if (arr.size() == 2 && !std::strncmp(arr[1].c_str(),"USER ", 5))
		this->userCommand(arr[1], client, cmd);
}

void Server::userCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "USER";
	if (line.size() < 6)
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
	else
	{
		std::vector<std::string> userList = split(line, ' ');
		if (userList.size() < 2)
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		else
			client.setUserName(userList[1]); // Check if we'll need to parse it.
	}
}