#include "../include/Server.hpp"

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
			throw std::runtime_error("Error en poll(): ");
		for (size_t i = 0; i < _fds.size() ; i++)
		{
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _serverSocket)
					newConnections();
				else
					eventMsg(_fds, i);
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

	Client newClient(clientSocket);
	_client.push_back(newClient);

	struct pollfd newPoll;

	newPoll.fd = clientSocket;
	newPoll.events = POLLIN | POLLOUT;
	newPoll.revents = 0;
	_fds.push_back(newPoll);
}
    
void Server::eventMsg(std::vector<struct pollfd> &fds, int i)
{
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int clientSocket = fds[i].fd;

	int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
	if (bytes <= 0)
	{
		close (fds[i].fd);
		fds.erase(fds.begin() + i);
		std::cout << "Client disconected." << std::endl;
	}
	std::cout << "El buffer es: " << buffer << "\n";
	parsedInput(buffer);
}

static void removeCarriageReturn(std::string &str)
{
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
}

void Server::parsedInput(std::string str)
{
    std::vector<std::string> ret;
    ret = split(str,'\n');
	if (ret.size() == 0) // We need to handle this error.
	{
		std::cout << "Error splitting buffer" << std::endl;
		return ;
	}
	for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); ++it)
		removeCarriageReturn(*it);
	ret.pop_back();
	this->checkCommand(ret);
}

void Server::joinChannel(std::vector<std::string> arr)
{
	if (arr.size() > 1)
	{
		
	}
	else
	{
		std::cout << "Error ERR_NEEDMOREPARAMS(461)" << std::endl;
	}
}

void Server::checkCommand(std::vector<std::string> arr)
{
	std::vector<std::string> aux;

	if (arr.size() > 1) // First input with client data.
	{
		size_t j = 0;
		size_t i = 0;
		while (i < arr.size())
		{
			aux = split(arr[i], ' ');
			while (j < aux.size())
			{
				if (aux[0] == "NICK")
					std::cout << "llega\n";
				j++;
			}
			i++;
		}
	}
	else
	{
		aux = split(arr[0],' ');
		if (aux[0] == "JOIN")
			joinChannel(aux);
		else if (aux[0] == "MODE")
		{
			std::cout << "Entra al MODE\n";
		}
		else
			std::cout << "Command not valid." << std::endl;
	}
}