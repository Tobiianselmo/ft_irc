
#include "../include/Server.hpp"

void	Server::quitCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "QUIT";
	std::vector<std::string>	parameters = split(line, ' ');
		// we'll need a remClientFromServ() function

	_clientsMap.erase(client.getClientSocket());
	for (int i = 0; i < (int)this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == client.getClientSocket())
		{
			this->_fds.erase(this->_fds.begin() + i);
			break ;
		}
	}
	for (int i = this->_channels.size() - 1; i >= 0; i--)
	{
		this->_channels[i].deleteClient(&client);
		if (this->_channels[i].getUserSize() == 0)
			this->_channels.erase(_channels.begin() + i);
	}
	//en teoria haciendo esto, se está eliminando tambien de los canales disponibles
	close(client.getClientSocket());
	// send(cmd.client->getClientSocket(), response.c_str(), response.size(), 0);
}