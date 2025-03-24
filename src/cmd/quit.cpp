#include "../../include/Server.hpp"

void	Server::quitCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "QUIT";
	cmd.msg = commandToUpper(line);
	std::vector<std::string>	parameters = split(line, ' ');

	_clientsMap.erase(client.getClientSocket());
	for (size_t i = 0; i < this->_fds.size(); i++)
	{
		if (this->_fds[i].fd == client.getClientSocket())
		{
			this->_fds.erase(this->_fds.begin() + i);
			break ;
		}
	}
	for (int i = this->_channels.size() - 1; i >= 0; i--)
	{
		if (this->_channels[i].isClient(client.getNickName()) == true)
		{
			cmd.channel = &this->_channels[i];
			this->createResponse(RPL_QUIT, cmd, ALL_CHANNEL);
			this->_channels[i].deleteClient(&client);
		}
		if (this->_channels[i].getUserSize() == 0)
			this->_channels.erase(_channels.begin() + i);
	}
	close(client.getClientSocket());
	std::cout << "Client disconected" << std::endl;
}