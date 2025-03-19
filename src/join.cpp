#include "../include/Server.hpp"

void Server::joinCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "JOIN";
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() < 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
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
			this->createResponse(ERR_BADCHANMASK, cmd);
			continue ;
		}
		tmp = this->getChannel(channelName);
		if (!tmp)
		{
			Channel newChannel(channelName);
			newChannel.addClient(client);
			newChannel.addOperator(client);
			_channels.push_back(newChannel);
			cmd.channel = &newChannel;
			this->createResponse(RPL_JOIN, cmd);
			this->createResponse(RPL_NAMREPLY, cmd);
			this->createResponse(RPL_ENDOFNAMES, cmd);
		}
		else
		{
			cmd.channel = tmp;
			if (tmp->isClient(client.getNickName()) == true) // Check the response here
				break ;
			else if (tmp->hasLimit() == true && tmp->getUserSize() == tmp->getUsersLimit())
				this->createResponse(ERR_CHANNELISFULL, cmd);
			else if (tmp->getInvite() == true && tmp->isInvited(client.getNickName()) == false)
				this->createResponse(ERR_INVITEONLYCHAN, cmd);
			else if ((tmp->getInvite() == true && tmp->isInvited(client.getNickName())) || (tmp->getInvite() == false && tmp->hasPassword() == false))
			{
				tmp->addClient(client);
				sendMsgToChannel(tmp, ":" + client.getNickName() + " JOIN " + cmd.channel->getName() + "\r\n");
				if (tmp->hasTopic() == true)
					this->createResponse(RPL_TOPIC, cmd);
				this->createResponse(RPL_NAMREPLY, cmd);
				this->createResponse(RPL_ENDOFNAMES, cmd);
				tmp->deleteInvited(client);
			}
			else if (tmp->hasPassword() == true)
			{
				for (bool added = false; added != true;)
				{
					if (keyList.empty())
					{
						this->createResponse(ERR_BADCHANNELKEY, cmd);
						break ;
					}
					else if (tmp->getPassword() == keyList[0])
					{
						tmp->addClient(client);
						sendMsgToChannel(tmp, ":" + client.getNickName() + " JOIN " + cmd.channel->getName() + "\r\n");
						if (tmp->hasTopic() == true)
							this->createResponse(RPL_TOPIC, cmd);
						this->createResponse(RPL_NAMREPLY, cmd);
						this->createResponse(RPL_ENDOFNAMES, cmd);
						added = true;
					}
					if (!keyList.empty())
						keyList.erase(keyList.begin());
				}
			}
		}
	}
}
