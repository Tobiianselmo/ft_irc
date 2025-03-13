#include "../include/Server.hpp"

Server::~Server()
{
	close(this->_serverSocket);
	std::map<int, Client *>::iterator it_beg = _clientsMap.begin();
	std::map<int, Client *>::iterator it_end = _clientsMap.end();
	std::cout << it_beg->second->getNickName() << std::endl;
	while (it_beg != it_end)
	{
		delete it_beg->second;
		it_beg++;
	}
	_clientsMap.clear();
}

Server::Server(int port,const std::string &password)
{
	this->_port = port;
	this->_password = password;
	this->_serverSocket = -1;
}

int Server::getPort() const { return this->_port; }
int	Server::getServerSocket() const { return this->_serverSocket; }
const std::string	&Server::getPassword() const { return this->_password; }
const std::string	&Server::getHostName() const { return this->_hostName; }

void	Server::setHostName(std::string hostname) { this->_hostName = hostname; }

bool Server::isDuplicated(std::string name)
{
	for (size_t i = 1; i < _fds.size(); i++)
	{
		if (_clientsMap[_fds[i].fd]->getNickName() == name)
			return true;
	}
	return false;
}

t_data Server::initStructure(std::string msg, Client &client)
{
	t_data ret;

	ret.msg = msg;
	ret.client = &client;
	ret.channel = NULL;
	return ret;
}

Channel *Server::getChannel(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return (&_channels[i]);
	}
	return (NULL);
}

void Server::setupServer()
{
	std::vector<char> buffer(100);
	if (gethostname(buffer.data(), buffer.size()) == -1)
		throw std::runtime_error("Error getting hostname");
	this->setHostName(buffer.data());

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
					eventMsg(_fds, i, *_clientsMap[_fds[i].fd]);
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

	Client *newClient = new Client(clientSocket, *this);
	_clientsMap[clientSocket] = newClient;

	struct pollfd newPoll;

	newPoll.fd = clientSocket;
	newPoll.events = POLLIN | POLLOUT;
	newPoll.revents = 0;
	_fds.push_back(newPoll);
}
    
void Server::eventMsg(std::vector<struct pollfd> &fds, int i, Client &client)
{
	std::vector<std::string> arr;
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(client.getClientSocket(), buffer, sizeof(buffer), 0);
	if (bytes <= 0) // Comprobar el error en 0 y -1 por separado.
	{
		close (client.getClientSocket());
		fds.erase(fds.begin() + i);
		_clientsMap.erase(client.getClientSocket());
		std::cout << "Client disconected." << std::endl;
	}
	//std::cout << "El buffer es: " << buffer << "\n";
	arr = parsedInput(buffer);
	if(arr.size() == 0)
	{
		std::cout << "Error splitting buffer" << std::endl;
		return ;
	}
	t_data cmd = initStructure(buffer, client);
	if (client.isAuth() == false)
		authClient(arr, client, cmd);
	else
		checkCommand(arr, client, cmd);
}

static void removeCarriageReturn(std::string &str)
{
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
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

void Server::authClient(std::vector<std::string> arr, Client &client, t_data &cmd)
{
	std::vector<std::string> aux;
	if (!std::strncmp(arr[0].c_str(), "PASS ", 5))
	{
		if (arr[0].c_str() + 5 != this->getPassword())
		{
			this->createResponse(ERR_PASSWDMISMATCH, cmd);
			return ;
		}
	}
	else if (!std::strncmp(arr[0].c_str(), "NICK ", 5))
	{
		const char *nick = checkNickName(arr[0].c_str() + 5);
		if (nick == NULL)
			return ;
		if (isDuplicated(nick) == true)
		{
			std::cout << "Error: NickName duplicated." << std::endl;
			return ;
		}
		client.setNickName(nick);
		client.setAuth(true);
		if (!std::strncmp(arr[1].c_str(),"USER ", 5))
		{
			std::vector<std::string> split_user = split(arr[1],' ');
			client.setUserName(split_user[1]);
		}
	}
}

void Server::checkCommand(std::vector<std::string> arr, Client &client, t_data &cmd)
{
	std::string command = arr[0].substr(0, arr[0].find(" "));
	if (command == "NICK") // Made only for a test
	{
		std::string nick = checkNickName(arr[0].c_str() + 5);
		if (nick.c_str() == NULL)
			return ;
		if (isDuplicated(nick) == true)
		{
			std::cout << "Error: NickName duplicated." << std::endl;
			return ;
		}
		client.setNickName(nick);
	}
	else if (command == "JOIN")
		this->joinCommand(arr[0], client, cmd);
	else if (command == "KICK")
		std::cout << kickCommand(arr[0],client);
	else if (command == "TOPIC")
		std::cout << topicCommand(arr[0],client);
	else if (command == "MODE")
		this->modes(arr[0], client);
	else if (command == "INVITE")
		this->inviteCommand(arr[0], client);
	else
		std::cout << "Command not valid." << std::endl;
}