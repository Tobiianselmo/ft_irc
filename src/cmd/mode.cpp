#include "../../include/Server.hpp"

int	Server::addChannelKey(t_data &cmd, std::vector<std::string> &line, int args)
{
	cmd.description = " k * :You must specify a parameter for the key mode. Syntax: <key>.\r\n";
	if (args >= (int)line.size())
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
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
	return (1);
}

int	Server::addChannelLimit(t_data &cmd, std::vector<std::string> &line, int args)
{
	cmd.description = " l * :You must specify a parameter for the limit mode. Syntax: <limit>.\r\n";
	if (args > (int)line.size())
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
	// Aca se podria chequear si hay algo que no sea numeros quizas.
	for (int i = 0; i < (int)line[args].size(); i++)
	{
		if (!isdigit(line[args][i]))
		{
			cmd.description = " l * :Invalid limit mode parameter. Syntax: <limit>.\r\n";
			this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
			return (0);
		}
	}
	cmd.channel->setLimit(atoi(line[args].c_str()));
	cmd.channel->setHasLimit(true);
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
	Client *clientTmp = cmd.channel->getClient(line[args]);
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
	cmd.description = " o * :You must specify a parameter for the op mode. Syntax: <nick>.\r\n";
	if (args > (int)line.size())
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.destUser = line[args];
	Client *clientTmp = cmd.channel->getClient(line[args]);
	if (!clientTmp)
	{
		this->createResponse(ERR_USERNOTINCHANNEL, cmd, ONLY_CLIENT);
		return (0);
	}
	if (cmd.channel->isOperator(clientTmp->getNickName()) == false)
		return (0);
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
		return (0);
	if (args >= (int)line.size())
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
	if (line[args] != cmd.channel->getPassword())
	{
		this->createResponse(ERR_INVALIDKEY, cmd, ONLY_CLIENT);
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
	cmd.description = " l * :You must specify a parameter for the limit mode. Syntax: <limit>.\r\n";
	if (cmd.channel->hasLimit() == false)
		return (0);
	if ((int)line.size() < args)
	{
		this->createResponse(ERR_INVALIDMODEPARAM, cmd, ONLY_CLIENT);
		return (0);
	}
	cmd.channel->setHasLimit(false);
	cmd.channel->setLimit(0); 
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
	int	add = 0;

	if (cmd.channel->isOperator(client.getNickName()) == false)
	{
		this->createResponse(ERR_CHANOPRIVSNEEDED, cmd, ONLY_CLIENT);
		return (0);
	}
	(*i)++;
	while (*i < (int)line[2].size())
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
				this->createResponse(RPL_NAMREPLY, cmd, ALL_CHANNEL);
				ret = 1;
			}
		}
		else if (line[2][*i] == 'k')
		{
			args++;
			if (this->addChannelKey(cmd, line, args))
			{
				if (!add)
				{
					add = 1;
					cmd.prefix += "+";
				}
				cmd.prefix += "k";
				// ret = 1;
			}
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
		else if (line[2][*i] == '-' || line[2][*i] == '+')
		{
			(*i)--;
			return (ret);
		}
		else if (!strchr("+-ioklt", line[2][*i]))
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
	(*i)++;
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
				ret = 1;
			}
		}
		else if (line[2][*i] == 'l')
		{
			args++;
			if (this->delChannelLimit(cmd, line, args))
			{
				if (!add)
				{
					add = 1;
					cmd.prefix += "-";
				}
				cmd.prefix += "l";
				ret = 1;
			}
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
			ret = 1;
		}
		else if (line[2][*i] == '+' || line[2][*i] == '-')
		{
			(*i)--;
			return (ret);
		}
		else if (!strchr("+-ioklt", line[2][*i]))
			this->createResponse(ERR_UNKNOWNMODE, cmd, ONLY_CLIENT);
		(*i)++;
	}	
	return (ret);
}

void	Server::modeCommand(std::string &line, Client &client, t_data &cmd)
{
	cmd.cmdType = "MODE";
	std::vector<std::string>	parameters = split(line, ' ');

	if (parameters.size() == 2)
	{
		cmd.channelName = parameters[1];
		cmd.channel = this->getChannel(parameters[1]);
		if (!cmd.channel)
		{
			this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
			return ;
		}
		cmd.channel->sendModes(cmd, RPL_CHANNELMODEIS);

		return ;
	}
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
	if (!cmd.channel->isClient(cmd.client->getNickName()))
	{
		this->createResponse(ERR_NOTONCHANNEL, cmd, ONLY_CLIENT);
		return ;
	}
	cmd.prefix = ":" + client.getNickName() + "!" + this->getHostName() + " MODE " + cmd.channel->getName() + " ";
	int added = 0;
	for (int i = 0; i < (int)parameters[2].size(); i++)
	{
		if (parameters[2][i] == '+')
			added = this->addMode(parameters, client, cmd, &i);
		else if (parameters[2][i] == '-')
			added = this->delMode(parameters, client, cmd, &i);
		else
			this->createResponse(ERR_INVALIDCHAR, cmd, ONLY_CLIENT);
	}
	if (cmd.suffix.size() > 0)
		cmd.prefix += " " + cmd.suffix;
	cmd.prefix += "\r\n";
	if (added)
		sendMsgToChannel(cmd.prefix, ALL_CHANNEL, cmd);
}
