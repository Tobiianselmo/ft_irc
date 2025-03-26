#include "../../include/Server.hpp"

void	playBot(std::string player,int fd)
{

	if(player.c_str() == NULL || player != "rock" || player != "paper" || player != "scissors")
		send(fd,"Invalid Option\r\n",16,0);
	std::string options[] = {"rock","paper","scissors"};
	std::string choose = options[rand() % 3];
	std::string msg;
	if (player == choose)
		msg = "Player = " + player + "\nBot = " + choose + "\nDraw!\r\n";
	else if((player == "rock" && choose == "paper") ||
			(player == "scissors" && choose == "rock") ||
			(player == "paper" && choose == "scissors"))
		msg = "Player = " + player + "\nBot = " + choose + "\nYou Lose!\r\n";
	else
		msg = "Player = " + player + "\nBot = " + choose + "\nYou Win!\r\n";
	send(fd,msg.c_str(),strlen(msg.c_str()),0);
}
void	help(int fd)
{
	send(fd,"Command Bot Instruction\r\n",25,0);
	send(fd,"Command play: play rock,paper,scissors\r\n",40,0);
}
void	Server::botCommand(std::string line, t_data &cmd)
{
	std::vector<std::string>arr = split(line, ' ');
	if(arr[1] == "play" || arr[1] == "PLAY")
		playBot(arr[2],cmd.client->getClientSocket());
	else if(arr[1] == "help" || arr[1] == "HELP")
		help(cmd.client->getClientSocket());
}