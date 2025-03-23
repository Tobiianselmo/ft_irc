#include "../../include/Server.hpp"

void Server::joinCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "JOIN";
	cmd.msg = commandToUpper(line);
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() < 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}

	std::vector<std::string> channelList = split(parameters[1], ',');
	std::vector<std::string> keyList;
	
	if (parameters.size() > 2)
	keyList = split(parameters[2], ',');

	Channel *tmp;

	for (size_t i = 0; i < channelList.size(); i++)
	{
		std::string channelName = channelList[i];
		cmd.channelName = channelName;
		if (channelName.empty() || !std::strchr("#&", channelName[0]))
		{
			this->createResponse(ERR_BADCHANMASK, cmd, ONLY_CLIENT);
			continue ;
		}
		tmp = this->getChannel(channelName);
		if (!tmp)
		{
			Channel newChannel(channelName);
			newChannel.addClient(&client);
			newChannel.addOperator(&client);
			_channels.push_back(newChannel);
			cmd.channel = &newChannel;
			this->createResponse(RPL_JOIN, cmd, ONLY_CLIENT);
			this->createResponse(RPL_NAMREPLY, cmd, ONLY_CLIENT);
			this->createResponse(RPL_ENDOFNAMES, cmd, ONLY_CLIENT);
		}
		else
		{
			cmd.channel = tmp;
			if (tmp->isClient(client.getNickName()) == true) // Check the response here
				break ;
			else if (tmp->hasLimit() == true && tmp->getUserSize() == tmp->getUsersLimit())
				this->createResponse(ERR_CHANNELISFULL, cmd, ONLY_CLIENT);
			else if (tmp->getInvite() == true && tmp->isInvited(client.getNickName()) == false)
				this->createResponse(ERR_INVITEONLYCHAN, cmd, ONLY_CLIENT);
			else if ((tmp->getInvite() == true && tmp->isInvited(client.getNickName())) || (tmp->getInvite() == false && tmp->hasPassword() == false))
			{
				tmp->addClient(&client);
				this->createResponse(RPL_JOIN, cmd, ALL_CHANNEL);
				if (tmp->hasTopic() == true)
					this->createResponse(RPL_TOPIC, cmd, ONLY_CLIENT);
				this->createResponse(RPL_NAMREPLY, cmd, ONLY_CLIENT);
				this->createResponse(RPL_ENDOFNAMES, cmd, ONLY_CLIENT);
				tmp->deleteInvited(&client);
			}
			else if (tmp->hasPassword() == true)
			{
				for (bool added = false; added != true;)
				{
					if (keyList.empty())
					{
						this->createResponse(ERR_BADCHANNELKEY, cmd, ONLY_CLIENT);
						break ;
					}
					else if (tmp->getPassword() == keyList[0])
					{
						tmp->addClient(&client);
						this->createResponse(RPL_JOIN, cmd, ALL_CHANNEL);
						if (tmp->hasTopic() == true)
							this->createResponse(RPL_TOPIC, cmd, ONLY_CLIENT);
						this->createResponse(RPL_NAMREPLY, cmd, ONLY_CLIENT);
						this->createResponse(RPL_ENDOFNAMES, cmd, ONLY_CLIENT);
						added = true;
					}
					if (!keyList.empty())
						keyList.erase(keyList.begin());
				}
			}
		}
	}
}
