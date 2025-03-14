
#include "../include/Server.hpp"

int	Server::addMode(std::vector<std::string> &line, Client &client, t_data &cmd)
{
	Channel *tmp = this->getChannel(line[1]);
	if (!tmp)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd);
		return (ERR_NOSUCHCHANNEL);
	}
	//este if es solo para los modes de canales
	if (tmp->isOperator(client.getNickName()) == false)//si el cliente que lo pide es operador
	{
		this->createResponse(ERR_NOOPERHOST, cmd);
		return (ERR_NOOPERHOST);//REVISAR ERROR y agregar mensaje de error
	}
	if (line[2] == "+i")
		tmp->setMode(line[2], true);
	else if (line[2] == "+o")
	{
		if (line.size() != 4)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return (ERR_NEEDMOREPARAMS);
		}
		Client *clientTmp = tmp->getClient(line[3]);//si el cliente que se quiere hacer operador es del canal
		if (!clientTmp)
		{
			this->createResponse(ERR_NOSUCHNICK, cmd);
			return (ERR_NOSUCHNICK);//poner mensaje de error
		}
		tmp->addOperator(*clientTmp);
		tmp->printOperators();
	}
	else if (line[2] == "+k") // hecho para testear el join (tobi)
	{
		if (line.size() < 4)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return (ERR_NEEDMOREPARAMS);
		}
		else if (line.size() > 4) // si la password tenia espacios
		{
			this->createResponse(ERR_INVALIDMODEPARAM, cmd);
			return (ERR_INVALIDMODEPARAM);
		}
		if (tmp->isOperator(client.getNickName()) == true)
		{
			tmp->setHasPassword(true);
			tmp->setPassword(line[3]);
			std::cout << "Password has changed to " << line[3] << std::endl;
		}
	}
	return (0);
}

int	Server::delMode(std::vector<std::string> &line, Client &client, t_data &cmd)
{
	(void)client;
	Channel *tmp = this->getChannel(line[1]);
	if (!tmp)
	{
		this->createResponse(ERR_NOSUCHCHANNEL, cmd);
		return (ERR_NOSUCHCHANNEL);
	}
	if (line[2] == "-i")
		tmp->setMode(line[2], false);
	else if (line[2] == "-o")
	{
		if (line.size() != 4)
		{
			this->createResponse(ERR_NEEDMOREPARAMS, cmd);
			return (ERR_NEEDMOREPARAMS);
		}
		if (tmp->isOperator(client.getNickName()) == false)//si el cliente que lo pide es operador
		{
			this->createResponse(ERR_NOOPERHOST, cmd);
			return (ERR_NOOPERHOST);
		}
		Client *clientTmp = tmp->getClient(line[3]);
		if (!clientTmp)
		{
			this->createResponse(ERR_NOSUCHNICK, cmd);
			return (ERR_NOSUCHNICK);
		}
		if (tmp->isOperator(clientTmp->getNickName()))
			tmp->deleteOperators(*clientTmp);
	}
	return (0);
}

void	Server::modes(std::string &line, Client &client, t_data &cmd)
{
	cmd.cmdType = "MODE";
	std::vector<std::string>	parameters = split(line, ' ');

	if (parameters.size() < 3)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd);
		return ;
	}
	if (parameters[2][0] == '+')
		this->addMode(parameters, client, cmd);
	else if (parameters[2][0] == '-')
		this->delMode(parameters, client, cmd);
	// this->_channels[0].getMode();
	return ;
}
