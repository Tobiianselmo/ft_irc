#include "../include/Server.hpp"

void Server::passCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "PASS";
	if (client.isAuth() == true)
		return ; // Add a response when the client tries to change the PASS, once he's auth.
	std::vector<std::string> splitParams = split(line, ' ');
	if (splitParams.size() < 2)
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
	else if (splitParams[1] != this->getPassword())
		this->createResponse(ERR_PASSWDMISMATCH, cmd);
	else
		client.setCorrectPass(true);
}

void Server::nickCommand(std::vector<std::string> arr, Client &client, t_data &cmd)
{
	cmd.cmdType = "NICK";
	std::vector<std::string> splitParams = split(arr[0], ' ');
	if (splitParams.size() < 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	const char *nick = checkNickName(splitParams[1].c_str());
	if (client.getNickName().size() == 0)
	{
		if (nick == NULL)
		{
			client.setNickName(splitParams[1]);
			this->createResponse(ERR_ERRONEUSNICKNAME, cmd);
		}
		else if (isDuplicated(splitParams[1]) == true)
		{
			client.setNickName(splitParams[1]);
			this->createResponse(ERR_NICKNAMEINUSE, cmd);
		}
		else
		{
			cmd.authMsg = arr[0];
			this->createResponse(RPL_NICKSUCCESS, cmd);
			client.setNickName(nick);
			client.setAuth(true);
		}
	}
	else
	{
		if (nick == NULL)
			this->createResponse(ERR_ERRONEUSNICKNAME, cmd);
		else if (isDuplicated(splitParams[1]) == true)
			this->createResponse(ERR_NICKNAMEINUSE, cmd);
		else
		{
			cmd.authMsg = arr[0];
			this->createResponse(RPL_NICKSUCCESS, cmd);
			client.setNickName(nick);
			client.setAuth(true);
		}
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