#include "../../include/Server.hpp"

void	Server::modeErrorMsg(t_data &cmd, std::string &msg)
{
	send(cmd.client->getClientSocket(), msg.c_str(), msg.size(), 0);
}

int	Server::addChannelKey(t_data &cmd, std::vector<std::string> &line, int args)
{
	if (args > (int)line.size())
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return (0);
	}
	// else if (line.size() > 4) // si la password tenia espacios
	// {
	// 	this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
	// 	return ;
	// }
	if (cmd.channel->hasPassword() == true)
	{
		this->createResponse(ERR_KEYSET, cmd, ONLY_CLIENT);//crear mensaje en el response
		return (0);
	}
	cmd.channel->setHasPassword(true);
	cmd.channel->setPassword(line[args]);
	if (cmd.suffix.size() > 0 || args == (int)line.size())
		cmd.suffix += ":";
	if (args != (int)line.size())
		cmd.suffix += line[args] + " ";
	else
		cmd.suffix += line[args];;
	return (1);
}
// int	Server::addChannelTopic()
// {

// }
int	Server::addChannelLimit(t_data &cmd, std::vector<std::string> &line, int args)
{
	if (cmd.channel->hasLimit() == true)
	{
		this->createResponse(ERR_LIMITSET, cmd, ONLY_CLIENT);//escribir mensaje
		return (0);
	}
	if (args > (int)line.size())
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return (0);
	}
	if (atoi(line[args].c_str()) < cmd.channel->getUserSize())
	{
		this->createResponse(ERR_INVALIDLIMIT, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.channel->setLimit(atoi(line[args].c_str()));
	cmd.channel->setHasLimit(true);//deberia haber mensaje?
	if (cmd.suffix.size() > 0 || args == (int)line.size())
		cmd.suffix += ":";
	if (args != (int)line.size())
		cmd.suffix += line[args] + " ";
	else
		cmd.suffix += line[args];
	return (1);
}

int	Server::addChannelOperator(t_data &cmd, std::vector<std::string> &line, int args)
{
	if (args > (int)line.size())
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return (0);
	}
	Client *clientTmp = cmd.channel->getClient(line[args]);//si el cliente que se quiere hacer operador es del canal
	if (!clientTmp)
	{
		this->createResponse(ERR_USERNOTINCHANNEL, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.channel->addOperator(clientTmp);
	if (cmd.suffix.size() > 0 || args == (int)line.size())
		cmd.suffix += ":";
	if (args != (int)line.size())
		cmd.suffix += line[args] + " ";
	else
		cmd.suffix += line[args];
	return (1);
}

int	Server::delChannelOperator(t_data &cmd, std::vector<std::string> &line, int args)
{
	if (args > (int)line.size())
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return (0);
	}
	Client *clientTmp = cmd.channel->getClient(line[args]);
	if (!clientTmp)
	{
		this->createResponse(ERR_USERNOTINCHANNEL, cmd, ONLY_CLIENT);
		return (0);
	}
	if (cmd.channel->isOperator(clientTmp->getNickName()))
		cmd.channel->deleteOperators(clientTmp);
	if (cmd.suffix.size() > 0 || args == (int)line.size())
		cmd.suffix += ":";
	if (args != (int)line.size())
		cmd.suffix += line[args] + " ";
	else
		cmd.suffix += line[args];
	return (1);
}
int	Server::delChannelKey(t_data &cmd, std::vector<std::string> &line, int args)
{
	if (cmd.channel->hasPassword() == false)
		return (0); //buscar error
	if (args > (int)line.size())
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return (0);
	}
	if (line[args] == cmd.channel->getPassword())
	{
		cmd.channel->setHasPassword(false);
		cmd.channel->deletePassword();
	}
	else
		;//mandar mensaje de error y de success
	if (cmd.suffix.size() > 0 || args == (int)line.size())
		cmd.suffix += ":";
	if (args != (int)line.size())
		cmd.suffix += line[args] + " ";
	else
		cmd.suffix += line[args];
	return (1);
}

int	Server::delChannelLimit(t_data &cmd, std::vector<std::string> &line, int args)
{
	if (cmd.channel->hasLimit() == false)
	{
		this->createResponse(ERR_NOLIMIT, cmd, ONLY_CLIENT);//escribir mensaje
		return (0);
	}
	if ((int)line.size() < args)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.channel->setHasLimit(false);
	cmd.channel->setLimit(10);
	if (cmd.suffix.size() > 0 || args == (int)line.size())
		cmd.suffix += ":";
	if (args != (int)line.size())
		cmd.suffix += line[args] + " ";
	else
		cmd.suffix += line[args];
	return (1);
}

int	Server::addMode(std::vector<std::string> &line, Client &client, t_data &cmd, int *i)
{
	//este if es solo para los modes de canales
	if (cmd.channel->isOperator(client.getNickName()) == false)//si el cliente que lo pide es operador
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd, ONLY_CLIENT);
		return (1);
	}
	int	args = 2;
	int	add = 0;
	std::string error;
	while ((*i) < (int)line[2].size())
	{
		if (line[2][*i] == 'i')
		{
			if (!add)
			{
				add = 1;
				cmd.prefix += "+";
			}
			cmd.channel->setInviteOnly(true);
			cmd.prefix += "i";
		}
		else if (line[2][*i] == 'o')
		{
			args++;
			// #42 o * :You must specify a parameter for the op mode. Syntax: <nick>.
			if (!this->addChannelOperator(cmd, line, args))
			{
				error = ":ft_irc 696 " + cmd.client->getNickName() + " " + cmd.channel->getName() + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
				this->modeErrorMsg(cmd, error);
				return (1);
			}
			if (!add)
			{
				add = 1;
				cmd.prefix += "+";
			}
			cmd.prefix += "o";
			//Debería tener un print cuando alguien se convierte en operador? si 
		}
		else if (line[2][*i] == 'k') // hecho para testear el join (tobi)
		{
			args++;
			// #42 k * :You must specify a parameter for the key mode. Syntax: <key>.
			if (!this->addChannelKey(cmd, line, args))
			{
				error = ":ft_irc 696 " + cmd.client->getNickName() + " " + cmd.channel->getName() + " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n";
				this->modeErrorMsg(cmd, error);
				return (1);
			}
			if (!add)
			{
				add = 1;
				cmd.prefix += "+";
			}
			//agregar mensaje
			cmd.prefix += "k";
		}
		else if (line[2][*i] == 'l')
		{
			args++;
			if (!this->addChannelLimit(cmd, line, args))
			{
				error = ":ft_irc 696 " + cmd.client->getNickName() + " " + cmd.channel->getName() + " l *:You must specify a parameter for the limit mode. Syntax: <limit>.\r\n";
				this->modeErrorMsg(cmd, error);
				return (1);
			}
			if (!add)
			{
				add = 1;
				cmd.prefix += "+";
			}
			cmd.prefix += "l";//AGREGAR MENSAJE TAMBIEN
		}
		else if (line[2][*i] == 't')
		{
			if (!add)
			{
				add = 1;
				cmd.prefix += "+";
			}
			cmd.channel->setAllowedTopic(true);
			cmd.prefix += "t";
		}
		else if (line[2][*i] == '-')
		{
			(*i)--;
			return (0);
		}
		else if (line[2][*i] != '+')
			this->createResponse(ERR_UNKNOWNMODE, cmd, ONLY_CLIENT);
		(*i)++;
	}	
	return (0);
}

int	Server::delMode(std::vector<std::string> &line, Client &client, t_data &cmd, int *i)
{
	int args = 2;
	int add = 0;

	if (cmd.channel->isOperator(client.getNickName()) == false)
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd, ONLY_CLIENT);
		return (1);
	}
	while (*i < (int)line[2].size())
	{
		if (line[2][*i] == 'i')
		{
			if (!add)
			{
				add = 1;
				cmd.prefix += "-";
			}
			cmd.channel->setInviteOnly(false);
			cmd.prefix += "i";
		}
		else if (line[2][*i] == 'o')
		{
			args++;
			if (!this->addChannelOperator(cmd, line, args))
				return (1);//agregar mensaje de (*i)++;error
			if (!add)
			{
				add = 1;
				cmd.prefix += "-";
			}
			cmd.prefix += "o";
		}
		else if (line[2][*i] == 'k')
		{
			args++;
			if (!this->delChannelKey(cmd, line, args))
				return (1); //agregar mensakes
			if (!add)
			{
				add = 1;
				cmd.prefix += "-";
			}
			cmd.prefix += "k";
		}
		else if (line[2][*i] == 'l')
		{
			args++;
			if (!this->delChannelLimit(cmd, line, args))
				return (1);//agregar menaje de error y revisar a qué se tendría que setear el limit cuando no hay mode porque ahora esta en 10
			if (!add)
			{
				add = 1;
				cmd.prefix += "-";
			}
			cmd.prefix += "l";
		}
		else if (line[2][*i] == 't')
		{
			if (!add)
			{
				add = 1;
				cmd.prefix += "-";
			}
			cmd.channel->setAllowedTopic(false);
			cmd.prefix += "t";
		}
		else if (line[2][*i] == '+')
		{
			(*i)--;
			break ;
		}	
		else if (line[2][*i] == '-')
			this->createResponse(ERR_UNKNOWNMODE, cmd, ONLY_CLIENT);
		(*i)++;
	}	
	return (0);
}

void	Server::modes(std::string &line, Client &client, t_data &cmd)
{
	cmd.cmdType = "MODE";
	std::vector<std::string>	parameters = split(line, ' ');

	if (parameters.size() < 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}
	cmd.channel = this->getChannel(parameters[1]);
	if (!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}
	if (parameters.size() == 2)
	{
		cmd.channel->sendModes(cmd, 324);
		return ;
	}
	cmd.prefix = ":" + client.getNickName() + "!" + this->getHostName() + " MODE " + cmd.channel->getName() + " ";
	int error = 0;
	for (int i = 0; i < (int)parameters[2].size(); i++)
	{
		if (parameters[2][i] == '+')
			error = this->addMode(parameters, client, cmd, &i);
		else if (parameters[2][i] == '-')
			error = this->delMode(parameters, client, cmd, &i);
		else
			this->createResponse(ERR_INVALIDCHAR, cmd, ONLY_CLIENT);
	}
	if ((int)cmd.suffix.size() > 0)
		cmd.prefix += " " + cmd.suffix;
	cmd.prefix += "\r\n";
	if (!error)
		send(cmd.client->getClientSocket(), cmd.prefix.c_str(), cmd.prefix.size(), 0);
	// this->_channels[0].getMode();
	return ;
}
