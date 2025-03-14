
#include "../include/Server.hpp"

int	Server::inviteCommand(std::string line, Client &client)
{
	std::vector<std::string> parameters = split(line, ' ');

	if (parameters.size() != 3)
		return (ERR_NEEDMOREPARAMS);
	Channel	*tmp = getChannel(parameters[2]);
	if (!tmp)
		return (ERR_NOSUCHCHANNEL);
	if (tmp->isOperator(client.getNickName()) == false)
		return (ERR_NOOPERHOST);
	Client	*clientTmp = this->getClient(parameters[1]);
	if (!clientTmp)
		return (ERR_NOSUCHNICK);
	tmp->addInvite(*clientTmp);
	tmp->printInvited();
	return (0);
}
