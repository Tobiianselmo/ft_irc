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
			this->createResponse(RPL_QUIT, cmd, ALL_CHANNEL);
	}
	this->remClientFromServ(client,aux);
	std::cout << "Client disconected" << std::endl;
}