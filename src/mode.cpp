
#include "../include/Server.hpp"

void	Channel::setMode(std::string mode, bool choice)
{
	if (mode == "+i" || mode == "-i")
		this->_inviteOnly = choice;
}

void	Channel::getMode() const
{
	std::cout << "Invite only Mode set to = " << this->_inviteOnly << std::endl;
}

bool	Channel::getInvite() const { return this->_inviteOnly; }

Client	*Channel::getClients(std::string nick)
{
	for (size_t i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i].getNickName() == nick)
			return (&this->_clients[i]);
	}
	return (NULL);
}

int	Server::addMode(std::vector<std::string> &line, Client &client)
{
	Channel *tmp = this->getChannel(line[1]);
	if (!tmp)
		return (ERR_NOSUCHCHANNEL);
	//este if es solo para los modes de canales
	if (tmp->isOperator(client.getNickName()) == false)//si el cliente que lo pide es operador
		return (ERR_NOOPERHOST);//REVISAR ERROR y agregar mensaje de error
	if (line[2] == "+i")
		tmp->setMode(line[2], true);
	else if (line[2] == "+o")
	{
		if (line.size() != 4)
			return (ERR_NEEDMOREPARAMS);
		Client *clientTmp = tmp->getClients(line[3]);//si el cliente que se quiere hacer operador es del canal
		if (!clientTmp)
			return (ERR_NOSUCHNICK);//poner mensaje de error
		tmp->addOperator(*clientTmp);
		tmp->printOperators();
	}
	else if (line[2] == "+k") // hecho para testear el join (tobi)
	{
		if (line.size() < 4)
			return (ERR_NEEDMOREPARAMS);
		else if (line.size() > 4) // si la password tenia espacios
			return (ERR_INVALIDMODEPARAM);
		if (tmp->isOperator(client.getNickName()) == true)
		{
			tmp->setHasPassword(true);
			tmp->setPassword(line[3]);
			std::cout << "Password has changed to " << line[3] << std::endl;
		}
	}
	return (0);
}

void	Channel::deleteOperators(const Client &client)
{
	int cont;

	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i].getNickName() == client.getNickName())
			cont = i;
	}
	this->_operators.erase(this->_operators.begin() + cont);
}

int	Server::delMode(std::vector<std::string> &line, Client &client)
{
	(void)client;
	Channel *tmp = this->getChannel(line[1]);
	if (!tmp)
		return (ERR_NOSUCHCHANNEL);
	if (line[2] == "-i")
		tmp->setMode(line[2], false);
	else if (line[2] == "-o")
	{
		if (line.size() != 4)
			return (ERR_NEEDMOREPARAMS);
		if (tmp->isOperator(client.getNickName()) == false)//si el cliente que lo pide es operador
			return (ERR_NOOPERHOST);
		Client *clientTmp = tmp->getClients(line[3]);
		if (!clientTmp)
			return (ERR_NOSUCHNICK);
		if (tmp->isOperator(clientTmp->getNickName()))
			tmp->deleteOperators(*clientTmp);
	}
	return (0);
}

void	Channel::printOperators()
{
	for (size_t i = 0; i < this->_operators.size(); i++)
	{
		std::cout << "operators " << this->_operators[i].getNickName() << std::endl;
	}
}

int	Server::modes(std::string &line, Client &client)
{
	std::vector<std::string>	parameters = split(line, ' ');
	int	status = 0;

	if (parameters.size() < 3)
		return (ERR_NEEDMOREPARAMS);
	if (parameters[2][0] == '+')
		status = this->addMode(parameters, client);
	else if (parameters[2][0] == '-')
		status = this->delMode(parameters, client);
	// this->_channels[0].getMode();
	return (status);
}
