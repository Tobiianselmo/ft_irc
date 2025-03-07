#include "../include/Server.hpp"

int Server::kickCommand(std::vector<std::string> arr,Client &client)
{
	(void)client;
	std::vector<std::string> split_users;
	std::string channel = arr[2];
	size_t i = 0;
	if (arr.size() < 1)
		return(ERR_NEEDMOREPARAMS); // REVISAR PORQUE EL HEXCHAT LARGA POR DEFECTO SU ERROR
	while(i < this->_channels.size())
	{
		if(this->_channels[i].getName() == channel)
			break;
		i++;
	}
	if(i == this->_channels.size())
	{
		std::cout << "Channel not found\n";
		return(ERR_NOSUCHCHANNEL);
	}
	split_users = split(arr[3].c_str() + 1,',');
	std::vector<Client> aux = this->_channels[i].getArrClients();
	size_t j = 0;
	size_t k = 0;
	std::cout << "tamaño de los usuarios: " << split_users.size() << std::endl;
	std::cout << "primer usuario: " << split_users[0] << std::endl;
	std::cout << "tamaño de la cantidad de usuarios en el canal: " << aux.size() << std::endl;
	std::cout << "usuario en el canal: " << aux[k].getNickName() << std::endl;
	while(j < split_users.size())
	{
		k = 0;
		while(k < aux.size())
		{
			if(split_users[j] == aux[k].getNickName())
			{
				std::cout << "ENTRANDOoooooooooooo\n";
				aux.erase(aux.begin() + k); ///PREGUNTAR A TOBI SI ESTO SE ESTA BORRANDO EN UNA COPIA DEL VECTOR DE CLIENTES DEL CANAL
				break ;
			}
			else if(split_users.size() - 1 == j)
			{
				std::cout << "ERR_USERNOTINCHANNEL\n"; // CUANDO INTENTA SACAR UN USUARIO Y NO PERTENECE AL CANAL
				ERR_NOTONCHANNEL; // CUANDO EL CLIENTE QUE EJECUTO EL COMANDO KICK NO PERTENECE AL CANAL
			}
			k++;
		}
		j++;
		std::cout << "valor de j:" << j << std::endl;
	}
	return(0);
}