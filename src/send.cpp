#include "../include/Server.hpp"

void	sendMsgToChannel(Channel *channel, std::string msg)
{
	std::vector<Client> clientsList = channel->getArrClients();
	for (size_t i = 0; i < clientsList.size(); i++)
		send(clientsList[i].getClientSocket(), msg.c_str(), msg.size(), 0);
}
void	Server::sendMsgToServer(std::string msg)
{
	for (size_t i = 0; i < _fds.size(); i++)
		send(_fds[i].fd, msg.c_str(), msg.size(), 0);
}