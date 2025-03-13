#include "../include/Server.hpp"

int Server::joinCommand(std::string line, Client &client)
{
	t_data data;
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() < 2)
		this->createResponse(ERR_NEEDMOREPARAMS, client, NULL);

	std::vector<std::string> channelList = split(parameters[1], ',');
	std::vector<std::string> keyList;

	if (parameters.size() > 2)
		keyList = split(parameters[2], ',');

	Channel *tmp;

	for (size_t i = 0; i < channelList.size(); i++)
	{
		std::string channelName = channelList[i];
		data.Channel = channelName;
		if (channelName.empty() || !std::strchr("#&", channelName[0]))
		{
			this->createResponse(ERR_BADCHANMASK, client, &data);
			continue;
		}
		tmp = this->getChannel(channelName);
		if (!tmp)
		{
			Channel newChannel(channelName);
			newChannel.addClient(client);
			newChannel.addOperator(client);
			_channels.push_back(newChannel);
			//data.Channel = newChannel.getName();
			// std::cout << client.getNickName() << " has correctly join " << newChannel.getName() << std::endl;
			this->createResponse(RPL_JOIN, client, &data);
			this->createResponse(RPL_NAMREPLY, client, &data);
			this->createResponse(RPL_ENDOFNAMES, client, &data);
		}
		else
		{
			if (tmp->isClient(client.getNickName()) == true)
				break ;
			else if (tmp->getInvite() == true && tmp->isInvited(client.getNickName()) == false)
				this->createResponse(ERR_INVITEONLYCHAN, client, &data);
			else if ((tmp->getInvite() == true && tmp->isInvited(client.getNickName())) || (tmp->getInvite() == false && tmp->hasPassword() == false))
			{
				tmp->addClient(client);
				sendMsgToChannel(tmp, ":" + client.getNickName() + " JOIN " + data.Channel + "\r\n");
				this->createResponse(RPL_NAMREPLY, client, &data);
				this->createResponse(RPL_ENDOFNAMES, client, &data);
			}
			else if (tmp->hasPassword() == true)
			{
				if (tmp->getPassword() == keyList[0])
				{
					tmp->addClient(client);
					this->createResponse(RPL_JOIN, client, &data);
					this->createResponse(RPL_NAMREPLY, client, &data);
					this->createResponse(RPL_ENDOFNAMES, client, &data);
				}
				else
					this->createResponse(ERR_BADCHANNELKEY, client, &data);
				if (!keyList.empty())
					keyList.erase(keyList.begin());
			}
		}
	}
	return (0);
}
