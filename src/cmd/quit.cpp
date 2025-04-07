#include "../../include/Server.hpp"

void	Server::quitCommand(std::string line, Client &client, t_data &cmd)
{
	cmd.cmdType = "QUIT";
	cmd.msg = commandToUpper(line);
	std::cout << "llega 1\n";
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
	std::cout << "llega 2\n";
	for (int i = this->_channels.size() - 1; i >= 0; i--)
	{
		std::cout << "llega 3\n";
		if (this->_channels[i].isClient(client.getNickName()) == true)
		{
			std::cout << "llega 4\n";
			this->createResponse(RPL_QUIT, cmd, ALL_CHANNEL);
			std::cout << "llega 5\n";
		}
	}
	this->remClientFromServ(client,aux);
	setCheckQuit(true);
	std::cout << "Client disconected" << std::endl;
}