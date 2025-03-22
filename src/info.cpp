#include "../include/Server.hpp"

void	Server::infoCommand(t_data &cmd)
{
	send(cmd.client->getClientSocket(),"cmd join or JOIN: join a channel\n",33,0);
	send(cmd.client->getClientSocket(),"cmd kick or KICK: kicks a user from a channel\n",46,0);
	send(cmd.client->getClientSocket(),"cmd nick or NICK: change nickname\n",34,0);
	send(cmd.client->getClientSocket(),"cmd mode or MODE: \n",19,0);
	send(cmd.client->getClientSocket(),"cmd invite or INVITE: \n",23,0);
	send(cmd.client->getClientSocket(),"cmd topic or TOPIC: change the channel topic\n",45,0);
	send(cmd.client->getClientSocket(),"cmd user or USER: change username\n",34,0);
	send(cmd.client->getClientSocket(),"cmd quit or QUIT: \n",19,0);
	send(cmd.client->getClientSocket(),"cmd part or PART: \n",19,0);
	send(cmd.client->getClientSocket(),"cmd pass or PASS: \n",19,0);
	send(cmd.client->getClientSocket(),"cmd privmsg or PRIVMSG: \n",25,0);
}