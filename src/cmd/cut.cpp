#include "../../include/Server.hpp"

void	Server::cutCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "CUT";
	std::vector<std::string>	parameters = split(line, ' ');

	if (parameters.size() != 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	cmd.channel = this->getChannel(parameters[1]);
	if (!cmd.channel)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd);
		return ;
	}
	if (cmd.channel->isClient(client.getNickName()) == false)
	{
		this->createResponse(ERR_USERNOTINCHANNEL, cmd);
		return ;
	}
	std::vector<Client>	clients = cmd.channel->getArrClients();
	if (cmd.channel->isOperator(client.getNickName()) == true)
	{
		if (clients.size() > 1)
		{
			cmd.channel->addOperator(clients[1]);
			send(clients[1].getClientSocket(), ": you are channel operator now.", 32, 0);
		}
	}
	if (clients.size() > 1)
		sendMsgToChannel(cmd.channel, ":" + client.getNickName() + " " + line + "\r\n");
	else if (clients.size() == 1)
	{
		for (int i = 0; i < (int)this->_channels.size(); i++)
		{
			if (this->_channels[i].getName() == cmd.channel->getName())
			{
				this->_channels.erase(_channels.begin() + i);
				break;
			}
		}
		std::cout << "left" << std::endl;
		return ;
	}		
	cmd.channel->deleteClient(client);
}