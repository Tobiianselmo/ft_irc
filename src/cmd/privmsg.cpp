#include "../../include/Server.hpp"

/*
	Params: PRIVMSG (USUARIO A QUIEN ENVIA),(SI SON VARIOS) (MENSAJE)

	Chequear si es un canal o un usuario con el primer caracter.

	Chequear que el cliente no es baneado del canal.

	Si hay un error es el ERR_CANNOTSENDTOCHAN (404)


	CREAR RESPUESTAS:
	ERR_CANNOTSENDTOCHAN
	ERR_NOTEXTTOSEND


	FINALIZAR

*/

void Server::privmsgCommand(std::string line, Client &client, t_data &cmd)
{
	(void)client;
	cmd.cmdType = "PRIVMSG";
	cmd.msg = commandToUpper(line);
	std::vector<std::string> parameters = split(line, ' ');
	
	if (parameters.size() < 2)
	{
		this->createResponse(ERR_NEEDMOREPARAMS, cmd, ONLY_CLIENT);
		return ;
	}
	
	std::vector<std::string> targetList = split(parameters[1], ',');
	if (parameters.size() < 3)
	{
		this->createResponse(ERR_NOTEXTTOSEND, cmd, ONLY_CLIENT);
		return ;
	}
	std::string message = join(parameters.begin() + 2, " ", parameters.size() - 2);

	for (size_t i = 0; i < targetList.size(); i++)
	{
		std::string target = targetList[i];
		cmd.destUser = target;
		if (target.empty() || (!isalpha(target[0]) && !strchr("#&@", target[0])))
		{
			this->createResponse(ERR_NOSUCHNICK, cmd, ONLY_CLIENT);
			continue ;
		}
		cmd.privMessage = message;
		if (strchr("#&@", target[0]))
		{
			if (target[0] == '@' && target.size() > 1) // Will send the msg only to the operators
			{
				cmd.channel = this->getChannel(&target[1]);
				cmd.destUser = &target[1];
				if (!cmd.channel)
					this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
				else
					this->createResponse(RPL_PRIVMSGSUCCESS, cmd, ONLY_OPERATORS);
			}
			else
			{
				cmd.channel = this->getChannel(target);
				if (!cmd.channel)
					this->createResponse(ERR_NOSUCHCHANNEL, cmd, ONLY_CLIENT);
				else
					this->createResponse(RPL_PRIVMSGSUCCESS, cmd, ALL_CHANNEL);
			}
		}
		else // If it is a client
		{
			Client *tmp = this->getClient(target);
			if (!tmp)
				this->createResponse(ERR_NOSUCHNICK, cmd, ONLY_CLIENT);
			else
				this->createResponse(RPL_PRIVMSGSUCCESS, cmd, ONE_CLIENT);
		}
	}
}