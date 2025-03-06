#include "../include/Server.hpp"

Server::Server(){}

Server::~Server()
{
	close(this->_serverSocket);
}

Server::Server(int port,const std::string &password)
{
	this->_port = port;
	this->_password = password;
	this->_serverSocket = -1;
}

int Server::getPort() const { return this->_port; }
int	Server::getServerSocket() const { return this->_serverSocket; }
const std::string &Server::getPassword() const { return this->_password; }

void Server::setupServer()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1)
		throw std::runtime_error("Error creating server socket");
	
	int opt = 1;
	if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(_serverSocket);
		throw std::runtime_error("Error setting options to server socket");
	}
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_serverSocket);
		throw (std::runtime_error("Failed to set non-blocking mode on server socket"));
	}
	
	std::memset(&this->_serverAddress, 0, sizeof(this->_serverAddress));
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(_port);
	
	if (bind(_serverSocket, (struct sockaddr*)&this->_serverAddress, sizeof(this->_serverAddress)) == -1)
	{
		close(_serverSocket);
		throw std::runtime_error("Error trying to bind server socket");
	}
	if (listen(_serverSocket, SOMAXCONN) == -1)
	{
		close(_serverSocket);
		throw std::runtime_error("Error trying to listen server socket");
	}

	std::cout << "Server listening in port " << _port << std::endl; // Delete

	struct pollfd fd;

	fd.fd = _serverSocket;
	fd.events = POLLIN;
	fd.revents = 0;
	_fds.push_back(fd);
}

void Server::handleConnections()
{
	while (true)
	{
		int ret = poll(&_fds[0], _fds.size(), -1);
		if (ret == -1)
			throw std::runtime_error("Error in poll(): ");
		for (size_t i = 0; i < _fds.size() ; i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverSocket)
					newConnections();
				else
					eventMsg(_fds, i, _clientsMap[_fds[i].fd]);
			}
		}
	}
}

void Server::newConnections()
{
	struct sockaddr_in clientAddress;
	socklen_t length = sizeof(clientAddress);
	int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddress, &length);

	if (clientSocket == -1)
		std::cerr << "Error handling connection"<< std::endl;
	int error = fcntl(clientSocket, F_GETFL, 0);
	if (error == -1 || fcntl(clientSocket, F_SETFL, error | O_NONBLOCK) == -1)
	{
		close(clientSocket);
		return ;
	}

	std::cout << "New client conected" << std::endl;
	
	Client newClient(clientSocket,*this);
	_client.push_back(newClient);
	
	struct pollfd newPoll;
	
	newPoll.fd = clientSocket;
	newPoll.events = POLLIN | POLLOUT;
	newPoll.revents = 0;
	std::cout << "conexion nueva fd: " << clientSocket << std::endl;
	_clientsMap[clientSocket] = newClient;
	_fds.push_back(newPoll);
}
    
void Server::eventMsg(std::vector<struct pollfd> &fds, int i, Client &client)
{
	std::vector<std::string> arr;
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	// std::cout << "llega fd :" << client.getClientSocket() << std::endl;
	int bytes = recv(fds[i].fd, buffer, sizeof(buffer), 0);
	if (bytes <= 0) // Comprobar el error en 0 y -1 por separado.
	{
		close(fds[i].fd);
		fds.erase(fds.begin() + i);
		//_clientsMap.erase(client.getClientSocket());
		std::cout << "Client disconected." << std::endl;
		return ;
	}
	std::cout << "El buffer es: " << buffer << "\n";
	arr = parsedInput(buffer);
	if(arr.size() == 0)
	{
		std::cout << "Error splitting buffer" << std::endl;
		return ;
	}
	this->checkCommand(arr, client);
}

std::vector<std::string> Server::parsedInput(std::string str)
{
	std::vector<std::string> ret;

	ret = split(str,'\n');
	if (ret.size() == 0) // We need to handle this error.
		return (ret);
	for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); ++it)
		removeCarriageReturn(*it);
	ret.pop_back();
	return(ret);
}

int Server::joinCommand(std::vector<std::string> arr, Client &client)
{
	(void)client;
	if (arr.size() > 1)
	{
		if (!std::strchr("#&", arr[1][0]))
			return (476); // Bad channel mask
		std::cout << "Client " << _client[0].getNickName() << " has joined to server " <<
		arr[1] << std::endl;
	}
	else
		return (461); // Needs more params
	return (0);
}
void Server::kickCommand(std::vector<std::string> arr,Client &client)
{
	std::vector<std::string> split_users;
	std::string channel = arr[1];
	size_t i = 0;
	while(i < this->_channels.size())
	{
		if(this->_channels[i].getName() == channel)
			break;
		i++;
	}
	if(i == this->_channels.size())
		std::cout << "Channel not found\n";
	split_users = split(arr[2],',');
	std::vector<Client> aux = this->_channels[i].getArrClients();
	size_t j = 0;
	size_t k;
	while(j < split_users.size())
	{
		k = 0;
		while(k < aux.size())
		{
			if(split_users[j] == aux[k].getNickName())

			k++;
		}
		j++;
	}
}
void Server::checkCommand(std::vector<std::string> arr, Client &client)
{
	std::vector<std::string> aux;
    if(!std::strncmp(arr[0].c_str(),"PASS ",5))
    {
        if(arr[0].c_str() + 5 != this->getPassword())
		{
			send(client.getClientSocket(),"Error: Incorrect Password\r\n",28,0);
			return ;
		}
    }
	if(arr.size() > 1) // First input with client data.
	{
/* 		size_t j = 0;
		size_t i = 0; */
        if(!std::strncmp(arr[0].c_str(),"NICK ",5))
		{
			std::string nick = checkNickName(arr[0].c_str() + 5);
			if (nick.c_str() == NULL)
				return ;
			for(size_t i = 1; i < this->_fds.size(); i++)
			{
				if(nick == _clientsMap[_fds[i].fd].getNickName())
				{
					std::cout << "error nick repetido y exit\n";
					return ;
				}
			}
			client.setNickName(nick);
		}
/* 		while (i < arr.size())
		{
			aux = split(arr[i], ' ');
			while (j < aux.size())
			{		
				j++;
			}
			i++;
		} */
	}
	else
	{
		aux = split(arr[0],' ');
		if (aux[0] == "JOIN")
			std::cout << "El output del join es: " << joinCommand(aux, client) << std::endl;
		else if(aux[0] == "KICK")
			kickCommand(aux,client);
		else if (aux[0] == "MODE")
		{	
			std::cout << "Entra al MODE\n";
		}
/* 		else
			std::cout << "Command not valid." << std::endl; */
	}
}