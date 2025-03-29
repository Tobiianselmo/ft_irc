#include "../../include/Server.hpp"

void	Server::infoCommand(t_data &cmd)
{
	send(cmd.client->getClientSocket(),"cmd join or JOIN: join a channel\n", 33, 0);
	send(cmd.client->getClientSocket(),"cmd kick or KICK: kicks a user from a channel\n", 46, 0);
	send(cmd.client->getClientSocket(),"cmd nick or NICK: change nickname\n", 34, 0);
	send(cmd.client->getClientSocket(),"cmd mode or MODE: modify the channel's modes\n", 45, 0);
	send(cmd.client->getClientSocket(),"cmd invite or INVITE: invites user to a channel\n", 48, 0);
	send(cmd.client->getClientSocket(),"cmd topic or TOPIC: change the channel topic\n", 45, 0);
	send(cmd.client->getClientSocket(),"cmd user or USER: change username\n", 34, 0);
	send(cmd.client->getClientSocket(),"cmd quit or QUIT: quit the server\n", 34, 0);
	send(cmd.client->getClientSocket(),"cmd part or PART: quit the channel\n", 35, 0);
	send(cmd.client->getClientSocket(),"cmd privmsg or PRIVMSG: send message to another user\n", 53, 0);
}
