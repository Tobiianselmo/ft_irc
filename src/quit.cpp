
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
	//en teoria haciendo esto, se está eliminando tambien de los canales dispnibles
	close(client.getClientSocket());
	// send(cmd.client->getClientSocket(), response.c_str(), response.size(), 0);
}