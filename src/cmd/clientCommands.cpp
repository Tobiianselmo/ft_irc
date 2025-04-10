#include "../../include/Server.hpp"

void Server::passCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "PASS";
	if (client.hasCorrectPass() == true)
	{
		this->createResponse(ERR_ALREADYREGISTERED, cmd, ONLY_CLIENT);
		return ;
	}
	std::vector<std::string> splitParams = split(line, ' ');
	if (splitParams.size() < 2)
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
	else if (splitParams[1] != this->getPassword())
		this->createResponse(ERR_PASSWDMISMATCH, cmd, ONLY_CLIENT);
	else
	{
		client.setCorrectPass(true);
		send(client.getClientSocket(),"PASS :Correct password\n",23,0);
		send(client.getClientSocket(),"Introduce the nickname(cmd NICK or nick)\n",41,0);
	}
}

void Server::nickCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "NICK";
	std::vector<std::string> splitParams = split(line, ' ');
	
	if (splitParams.size() != 2)
	{
		if (splitParams.size() > 2)
			this->createResponse(ERR_INVALIDPARAMS, cmd, ONLY_CLIENT);
		else
			this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}
	const char *nick = checkNickName(splitParams[1].c_str());
	if (client.getNickName().size() == 0)
	{
		if (nick == NULL)
		{
			client.setNickName(splitParams[1]);
			this->createResponse(ERR_ERRONEUSNICKNAME, cmd, ONLY_CLIENT);
		}
		else if (isDuplicated(splitParams[1]) == true)
		{
			client.setNickName(splitParams[1]);
			this->createResponse(ERR_NICKNAMEINUSE, cmd, ONLY_CLIENT);
		}
		else
		{
			cmd.authMsg = line;
			this->createResponse(RPL_NICKSUCCESS, cmd, ONLY_CLIENT);
			client.setNickName(nick);
			client.setAuth(true);
			send(client.getClientSocket(),"NICK :Correct NickName\n",23,0);
			send(client.getClientSocket(),"Introduce the UserName(cmd USER or user)\n",41,0);
		}
	}
	else
	{
		if (nick == NULL)
			this->createResponse(ERR_ERRONEUSNICKNAME, cmd, ONLY_CLIENT);
		else if (isDuplicated(splitParams[1]) == true)
			this->createResponse(ERR_NICKNAMEINUSE, cmd, ONLY_CLIENT);
		else
		{
			cmd.authMsg = line;
			this->createResponse(RPL_NICKSUCCESS, cmd, ONLY_CLIENT);
			if (this->getChannelsSize() > 0)
				this->sendMsgToAllChannels(client, cmd);
			client.setNickName(nick);
			client.setAuth(true);
			send(client.getClientSocket(),"NICK :Correct NickName\n",23,0);
			if (client.getUserName().size() == 0)
				send(client.getClientSocket(),"Introduce the UserName(cmd USER or user)\n",41,0);
		}
	}
}

void Server::userCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "USER";
	std::vector<std::string> splitParams = split(line, ' ');
	if (client.getNickName().size() <= 0)
	{
		send(client.getClientSocket(),"Introduce the nickname(cmd NICK or nick)\n",41,0);
		return ;
	}
	if (splitParams.size() < 2)
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
	else
	{
		send(client.getClientSocket(),"USER :Correct UserName\n",23,0);
		send(client.getClientSocket(),"---WELCOME TO THE SERVER---\n",28,0);
		send(client.getClientSocket(),"Introduce (cmd INFO or info)\n",29,0);
		std::cout << "Client Authenticated" << std::endl;
		client.setUserName(splitParams[1]); // Check if we'll need to parse it.
	}
}