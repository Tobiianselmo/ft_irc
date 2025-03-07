#include "../include/Server.hpp"

int Server::joinCommand(std::string line, Client &client)
{
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() < 2)
		return (461); // ERR_NEEDMOREPARAMS

	std::vector<std::string> channelList = split(parameters[1], ',');

	if (channelList.size() == 0)
		channelList.push_back(parameters[1]);

	std::vector<std::string> keyList;

	if (parameters.size() > 2)
		keyList = split(parameters[2], ',');

	Channel *tmp;

	for (size_t i = 0; i < channelList.size(); i++)
	{
		std::string channelName = channelList[i];

		std::cout << "entra " << channelName << std::endl;
		if (channelName.empty() || !std::strchr("#&", channelName[0]))
			return (476); // ERR_BADCHANMASK
		
		tmp = this->getChannel(channelName);
		if (!tmp)
		{
			Channel newChannel(channelName);
			newChannel.addClient(client);
			newChannel.addOperator(client);
			_channels.push_back(newChannel);
			std::cout << client.getNickName() << " has correctly join " << newChannel.getName() << std::endl;
		}
		else
		{
			if (tmp->isClient(client) == true)
				break ;
			tmp->addClient(client);
			std::cout << client.getNickName() << " has correctly join " << tmp->getName() << std::endl;
		}
	}
	return (0);
}