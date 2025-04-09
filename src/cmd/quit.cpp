#include "../../include/Server.hpp"

void	Server::quitCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "QUIT";
	cmd.msg = commandToUpper(line);
	int aux;
	std::vector<std::string>	parameters = split(line, ' ');

	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == client.getClientSocket())
		{
			aux = i;
			break ;
		}
	}
	for (int i = this->_channels.size() - 1; i >= 0; i--)
	{
		if (this->_channels[i].isClient(client.getNickName()) == true)
		{
			cmd.channel = &this->_channels[i];
			this->createResponse(RPL_QUIT, cmd, ALL_CHANNEL);
		}
	}
	if(this->_channels.size() > 0)
	{
		for (int i = this->_channels.size() - 1; i >= 0; i--)
		{
			cmd.channel = &this->_channels[i];
			bool opChange = false;
			if (cmd.channel->isOperator(client.getNickName()) && cmd.channel->getOperatorsSize() == 1)
			{
				std::vector<Client *>	clientAux = cmd.channel->getArrClients();
				for (int i = 0; i < (int)clientAux.size(); i++)
				{
					if (clientAux[i]->getNickName() != client.getNickName())
					{
						cmd.channel->addOperator(clientAux[i]);
						opChange = true;
						cmd.destUser = clientAux[i]->getNickName();
						break ;
					}
				}
			}
			this->createResponse(RPL_PART, cmd, ALL_CHANNEL);
			cmd.channel->deleteClient(&client);
			if (opChange)
				this->createResponse(RPL_OPERATOR, cmd, ALL_CHANNEL);
		}
	}
	this->remClientFromServ(client,aux);
	setCheckQuit(true);
	std::cout << "Client disconected" << std::endl;
}
