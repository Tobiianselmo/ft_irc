#include "../../include/Server.hpp"

void	Server::modeErrorMsg(t_data &cmd, std::string &msg)
{
	send(cmd.client->getClientSocket(), msg.c_str(), msg.size(), 0);
}

int	Server::addChannelKey(t_data &cmd, std::vector<std::string> &line, int args)
{
	cmd.description = " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n";
	if (args > (int)line.size())
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
	// else if (line.size() > 4)
	// {
	// 	this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
	// 	return (0);
	// }
	if (cmd.channel->hasPassword() == true)
	{
		this->createResponse(ERR_KEYSET, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.channel->setHasPassword(true);
	cmd.channel->setPassword(line[args]);
	if (cmd.suffix.size() > 0 || args == (int)line.size())
		cmd.suffix += ":";
	if (args != (int)line.size())
		cmd.suffix += line[args] + " ";
	else
		cmd.suffix += line[args];
	//mensaje de que esta todo correctou
	return (1);
}

int	Server::addChannelLimit(t_data &cmd, std::vector<std::string> &line, int args)
{
	// Este if no deberia estar, ya que el +l se puede usar en cualquier momento. por mas que ya este seteado
	// Si hago +l 5 y despues +l 8 el limite deberia cambiar a 8.

	// if (cmd.channel->hasLimit() == true)
	// {
	// 	this->createResponse(ERR_LIMITSET, cmd, ONLY_CLIENT);//escribir mensaje
	// 	return (0);
	// }
	if (args > (int)line.size())
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return (0);
	}

	// Este if tambien estaria de mas, ya que lo que ocurre si el limite es menor al total de usuarios
	// solamente no permite que se continuen uniendo usuarios al canal.

	// if (atoi(line[args].c_str()) < cmd.channel->getUserSize())
	// {
	// 	this->createResponse(ERR_INVALIDLIMIT, cmd, ONLY_CLIENT);
	// 	return (0);
	// }

	// Aca se podria chequear si hay algo que no sea numeros quizas.
	cmd.channel->setLimit(atoi(line[args].c_str()));
	cmd.channel->setHasLimit(true);//deberia haber mensaje? si
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
	cmd.description = " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
	if (args > (int)line.size())
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.destUser = line[args];
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
	cmd.destUser = line[args];
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
	cmd.description = " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n";
	if (cmd.channel->hasPassword() == false)
		return (0); //buscar error (NORMALMENTE NO HACE NADA XD)
	if (args > (int)line.size())
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
	if (line[args] != cmd.channel->getPassword())
	{
		this->createResponse(ERR_KEYSET, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.channel->setHasPassword(false);
	cmd.channel->deletePassword();
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
	int ret = 0;
	int	args = 2;
	int	add = 0; // Flag para saber si ya se agrego el + al prefix.

	//este if es solo para los modes de canales
	if (cmd.channel->isOperator(client.getNickName()) == false)//si el cliente que lo pide es operador
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd, ONLY_CLIENT);
		return (0);
	}
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
			ret = 1;
		}
		else if (line[2][*i] == 'o')
		{
			args++;
			if (this->addChannelOperator(cmd, line, args))
			{
				if (!add)
				{
					add = 1;
					cmd.prefix += "+";
				}
				cmd.prefix += "o";
				ret = 1;
			}
			// error = ":ft_irc 696 " + cmd.client->getNickName() + " " + cmd.channel->getName() + " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
			// this->modeErrorMsg(cmd, error);
			// return (1);
			//Debería tener un print cuando alguien se convierte en operador? si 
		}
		else if (line[2][*i] == 'k')
		{
			args++;
			if (this->addChannelKey(cmd, line, args)) // Aca se manejan las respuestas de error, no se donde se usaria el INVALIDMODEPARAM.
			{
				if (!add)
				{
					add = 1;
					cmd.prefix += "+";
				}
				cmd.prefix += "k";
				ret = 1;
			}
			// else
			// cmd.description = " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n";
			// this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		}
		else if (line[2][*i] == 'l')
		{
			args++;
			if (this->addChannelLimit(cmd, line, args))
			{
				if (!add)
				{
					add = 1;
					cmd.prefix += "+";
				}
				cmd.prefix += "l";
				ret = 1;
			}
			// error = ":ft_irc 696 " + cmd.client->getNickName() + " " + cmd.channel->getName() + " l *:You must specify a parameter for the limit mode. Syntax: <limit>.\r\n";
			// this->modeErrorMsg(cmd, error);
			// return (1);
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
			ret = 1;
		}
		else if (line[2][*i] == '-')
		{
			(*i)--;
			// Aca habria que ver que pasa si mando /MODE +-i, creeria que no va a largar problema, se podria comprobar aca.
			// Quizas el ERR_UNKNOWNMODE que esta abajo.
			return (ret);
		}
		else if (line[2][*i] != '+')
			this->createResponse(ERR_UNKNOWNMODE, cmd, ONLY_CLIENT);
		(*i)++;
	}
	return (ret);
}

int	Server::delMode(std::vector<std::string> &line, Client &client, t_data &cmd, int *i)
{
	int ret = 0;
	int args = 2;
	int add = 0;

	if (cmd.channel->isOperator(client.getNickName()) == false)
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd, ONLY_CLIENT);
		return (0);
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
			ret = 1;
		}
		else if (line[2][*i] == 'o')
		{
			args++;
			if (this->delChannelOperator(cmd, line, args))
			{
				if (!add)
				{
					add = 1;
					cmd.prefix += "-";
				}
				cmd.prefix += "o";
				ret = 1;
			}
		}
		else if (line[2][*i] == 'k')
		{
			args++;
			if (this->delChannelKey(cmd, line, args))
			{
				if (!add)
				{
					add = 1;
					cmd.prefix += "-";
				}
				cmd.prefix += "k";
			}
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
	cmd.channelName = parameters[1];
	cmd.channel = this->getChannel(parameters[1]);
	if (!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}
	if (parameters.size() == 2)
	{
		cmd.channel->sendModes(cmd, RPL_CHANNELMODEIS);
		return ;
	}
	cmd.prefix = ":" + client.getNickName() + "!" + this->getHostName() + " MODE " + cmd.channel->getName() + " ";
	int added = 0;
	for (int i = 0; i < (int)parameters[2].size(); i++)
	{
		if (parameters[2][i] == '+')
			added = this->addMode(parameters, client, cmd, &i);
		// else if (parameters[2][i] == '-')
		// 	added = this->delMode(parameters, client, cmd, &i);
		else
			this->createResponse(ERR_INVALIDCHAR, cmd, ONLY_CLIENT);
	}
	if (cmd.suffix.size() > 0)
		cmd.prefix += " " + cmd.suffix;
	cmd.prefix += "\r\n";
	if (added)
		sendMsgToChannel(cmd.prefix, ALL_CHANNEL, cmd);
}
