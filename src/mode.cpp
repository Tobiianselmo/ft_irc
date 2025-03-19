
#include "../include/Server.hpp"

void	Server::addMode(std::vector<std::string> &line, Client &client, t_data &cmd)
{
	//este if es solo para los modes de canales
	if (cmd.channel->isOperator(client.getNickName()) == false)//si el cliente que lo pide es operador
	{
		this->createResponse(ERR_NOOPERHOST, cmd);
		return ;
	}
	if (line[2] == "+i")
		cmd.channel->setInviteOnly(true);
	else if (line[2] == "+o")
	{
		if (line.size() != 4)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return ;
		}
		Client *clientTmp = cmd.channel->getClient(line[3]);//si el cliente que se quiere hacer operador es del canal
		if (!clientTmp)
		{
			this->createResponse(ERR_USERNOTINCHANNEL, cmd);
			return ;
		}
		cmd.channel->addOperator(*clientTmp);
		cmd.channel->printOperators();
	}
	else if (line[2] == "+k") // hecho para testear el join (tobi)
	{
		if (line.size() < 4)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return ;
		}
		else if (line.size() > 4) // si la password tenia espacios
		{
			this->createResponse(ERR_INVALIDMODEPARAM, cmd);
			return ;
		}
		if (cmd.channel->hasPassword() == true)
		{
			this->createResponse(ERR_KEYSET, cmd);//crear mensaje en el response
			return ;
		}
		cmd.channel->setHasPassword(true);
		cmd.channel->setPassword(line[3]);
		std::cout << "Password has changed to " << line[3] << std::endl;
	}
	else if (line[2] == "+l")
	{
		if (cmd.channel->hasLimit() == true)
		{
			this->createResponse(ERR_LIMITSET, cmd);//escribir mensaje
			return ;
		}
		if (line.size() < 4)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return ;
		}
		if (atoi(line[3].c_str()) < cmd.channel->getUserSize())
		{
			this->createResponse(ERR_INVALIDLIMIT, cmd);
			return ;
		}
		cmd.channel->setLimit(atoi(line[3].c_str()));
		cmd.channel->setHasLimit(true);
	}
	else
		this->createResponse(ERR_UNKNOWNMODE, cmd);
	return ;
}

void	Server::delMode(std::vector<std::string> &line, Client &client, t_data &cmd)
{
	if (cmd.channel->isOperator(client.getNickName()) == false)
	{
		this->createResponse(ERR_NOOPERHOST, cmd);
		return ;
	}
	if (line[2] == "-i")
		cmd.channel->setInviteOnly(false);
	else if (line[2] == "-o")
	{
		if (line.size() != 4)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return ;
		}
		Client *clientTmp = cmd.channel->getClient(line[3]);
		if (!clientTmp)
		{
			this->createResponse(ERR_NOSUCHNICK, cmd);
			return ;
		}
		if (cmd.channel->isOperator(clientTmp->getNickName()))
			cmd.channel->deleteOperators(*clientTmp);
	}
	else if (line[2] == "-k")
	{
		if (cmd.channel->hasPassword() == false)
			return ; //buscar error
		if (line.size() != 3)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return ;
		}
		cmd.channel->setHasPassword(false);
		cmd.channel->deletePassword();
	}
	else if (line[2] == "-l")
	{
		if (cmd.channel->hasLimit() == false)
		{
			this->createResponse(ERR_NOLIMIT, cmd);//escribir mensaje
			return ;
		}
		if (line.size() < 3)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return ;
		}
		cmd.channel->setHasLimit(false);
		cmd.channel->setLimit(10);
	}
	else
		this->createResponse(ERR_UNKNOWNMODE, cmd);
}

void	Server::modes(std::string &line, Client &client, t_data &cmd)
{
	cmd.cmdType = "MODE";
	std::vector<std::string>	parameters = split(line, ' ');

	if (parameters.size() < 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	cmd.channel = this->getChannel(parameters[1]);
	if (!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd);
		return ;
	}
	if (parameters.size() == 2)
	{
		cmd.channel->sendModes(cmd);
		return ;
	}
	if (parameters[2][0] == '+')
		this->addMode(parameters, client, cmd);
	else if (parameters[2][0] == '-')
		this->delMode(parameters, client, cmd);

	// this->_channels[0].getMode();
	return ;
}
