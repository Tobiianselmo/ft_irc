#include "../../include/Server.hpp"

void	playBot(std::string player, t_data &cmd)
{
	if (player != "rock" && player != "paper" && player != "scissors")
	{
		send(cmd.client->getClientSocket(), "Invalid Option\r\n", 16, 0);
		return ;
	}
	std::string options[] = {"rock", "paper", "scissors"};
	std::string choose = options[rand() % 3];
	std::string msg;
	if (player == choose)
		msg = cmd.client->getNickName() + " = " + player + "\nBot = " + choose + "\nDraw!\r\n";
	else if ((player == "rock" && choose == "paper")
			|| (player == "scissors" && choose == "rock")
			|| (player == "paper" && choose == "scissors"))
		msg = cmd.client->getNickName() + " = " + player + "\nBot = " + choose + "\nYou Lose!\r\n";
	else
		msg = cmd.client->getNickName() + " = " + player + "\nBot = " + choose + "\nYou Win!\r\n";
	send(cmd.client->getClientSocket(), msg.c_str(), strlen(msg.c_str()), 0);
}

void	help(int fd)
{
	send(fd, "Command Bot Instruction\r\n", 25, 0);
	send(fd, "Command play: play rock,paper,scissors\r\n",40,0);
}

void	Server::botCommand(std::string line, t_data &cmd)
{
	std::vector<std::string> arr = split(line, ' ');
	if (arr.size() == 1)
	{
		send(cmd.client->getClientSocket(), "Please enter a valid option: help or play\r\n",43,0);
		return ;
	}
	if (arr[1] == "play" || arr[1] == "PLAY")
	{
		if (arr.size() == 2)
			send(cmd.client->getClientSocket(), "Number arguments not valid\r\n", 28, 0);
		else
			playBot(arr[2], cmd);
	}
	else if (arr[1] == "help" || arr[1] == "HELP")
		help(cmd.client->getClientSocket());
}