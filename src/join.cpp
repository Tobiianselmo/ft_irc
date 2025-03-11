#include "../include/Server.hpp"

int Server::joinCommand(std::string line, Client &client)
{
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() < 2)
		return (461); // ERR_NEEDMOREPARAMS

	std::vector<std::string> channelList = split(parameters[1], ',');
	std::vector<std::string> keyList;

	if (parameters.size() > 2)
		keyList = split(parameters[2], ',');

	Channel *tmp;

	for (size_t i = 0; i < channelList.size(); i++)
	{
		std::string channelName = channelList[i];

		if (channelName.empty() || !std::strchr("#&", channelName[0]))
			return (476); // ERR_BADCHANMASK
		
		tmp = this->getChannel(channelName);
		if (!tmp)
		{
			Channel newChannel(channelName);
			newChannel.addClient(client);
			newChannel.addOperator(client);
			_channels.push_back(newChannel);
			// std::cout << client.getNickName() << " has correctly join " << newChannel.getName() << std::endl;
			this->createResponse(RPL_NAMREPLY, client, newChannel.getName());
		}
		else
		{
			if (tmp->isClient(client.getNickName()) == true)
				break ;
			if ((tmp->getInvite() == true && tmp->isInvited(client.getNickName())) || (tmp->getInvite() == false && tmp->hasPassword() == false)) //para verificar si el canal es invite only y si el cliente esta invitado o si no es invite only asi puede joinearse
			{
				tmp->addClient(client);
				std::cout << client.getNickName() << " has correctly join " << tmp->getName() << std::endl;
				this->createResponse(RPL_NAMREPLY, client, tmp->getName());
			}
			else if (tmp->getInvite() == true && tmp->isInvited(client.getNickName()) == false)
			{
				std::cerr << "Channel " << tmp->getName() << " is an Invite Only Channel." << std::endl; //chequear error y eliminar mensaje
				return (ERR_INVITEONLYCHAN);
			}
			else if (tmp->hasPassword() == true) // verificar si el canal es privado y le ha pasado la contraseña correcta.
			{
				bool added = false;
				for (size_t i = 0; i < keyList.size(); i++)
				{
					if (tmp->getPassword() == keyList[i])
					{
						tmp->addClient(client);
						added = true;
					}
				}
				if (added == false)
				{
					std::cout << "Error: client has not the correct key" << std::endl;
					return (ERR_BADCHANNELKEY);
				}
				else
					std::cout << client.getNickName() << " has correctly join " << tmp->getName() << std::endl;
				}
			// tmp->printChannel();
		}
	}
	return (0);
}
