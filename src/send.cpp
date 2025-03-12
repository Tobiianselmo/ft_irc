#include "../include/Server.hpp"

void	sendClient(Client &client,const char *str)
{
	send(client.getClientSocket(), str, std::strlen(str), 0);
}
void	sendChannel(Channel &channel,std::string str)
{
	(void)channel;
	(void)str;
}