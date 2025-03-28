#include "../include/Server.hpp"

Server::~Server()
{
	close(this->_serverSocket);
	std::map<int, Client *>::iterator it_beg = _clientsMap.begin();
	std::map<int, Client *>::iterator it_end = _clientsMap.end();
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

// Getters

int					Server::getPort() const { return this->_port; }
int					Server::getServerSocket() const { return this->_serverSocket; }
const std::string	&Server::getPassword() const { return this->_password; }
const std::string	&Server::getHostName() const { return this->_hostName; }
std::vector<struct pollfd>	Server::getFdsVector() const { return this->_fds; }

bool				Server::isDuplicated(std::string name)
{
	for (size_t i = 1; i < _fds.size(); i++)
	{
		if (_clientsMap[_fds[i].fd]->getNickName() == name)
			return true;
	}
	return false;
}

Channel				*Server::getChannel(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return (&_channels[i]);
	}
	return (NULL);
}

Client				*Server::getClient(std::string name)
{
	for (size_t i = 1; i < this->_fds.size(); i++)
	{
		if (_clientsMap[_fds[i].fd]->getNickName() == name)
			return (_clientsMap[_fds[i].fd]);
	}
	return (NULL);
}

void	Server::setHostName(std::string hostname) { this->_hostName = hostname; }

void	Server::remClientFromServ(Client &client, int i)
{
	_clientsMap.erase(client.getClientSocket());

	for (int i = this->_channels.size() - 1; i >= 0; i--)
	{
		this->_channels[i].deleteClient(&client);
		if (this->_channels[i].getUserSize() == 0)
			this->_channels.erase(_channels.begin() + i);
	}
	this->_fds.erase(this->_fds.begin() + i);
	close(client.getClientSocket());
}

t_data Server::initStructure(std::string msg, Client &client)
{
	t_data ret;

	ret.msg = msg;
	ret.client = &client;
	ret.channel = NULL;
	return ret;
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
					eventMsg(i, *_clientsMap[_fds[i].fd]);
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
	send(newPoll.fd,"Enter the password server(cmd PASS or pass)\n",44,0);
	_fds.push_back(newPoll);
}
    
// client._buffer = client.buffer.append(buffer)
// if (!client._buffer.find(\n))
// 	return;
//para señales, viene bien que el cliente tenga el buffer control d
//Ver errores de cuando hay muchos saltos de linea

//control z => suspende proceso. tiene que llegar los mensajes que se envien despues
// de que se suspende

void Server::eventMsg(int i, Client &client)
{
	std::vector<std::string> arr;
	char buffer[1024];
	std::memset(buffer, 0, sizeof(buffer));
	int bytes = recv(client.getClientSocket(), buffer, sizeof(buffer), 0);
	if (bytes < 0)
	{
		send(client.getClientSocket(), ":Error :recv function failed\r\n", 29, 0);
		return ;
	}
	else if (bytes == 0)
	{
		std::cout << "Client disconected." << std::endl;
		remClientFromServ(client, i);
		return ;
	}
	arr = parsedInput(buffer);
	if (arr.size() == 0)
	{
		std::cerr << "Error splitting buffer" << std::endl;
		return ;
	}
	for (size_t i = 0; i < arr.size(); i++)
	{	
		t_data cmd = initStructure(arr[i], client);
		checkCommand(arr[i], client, cmd);
	}
}

static void removeCarriageReturn(std::string &str)
{
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
}

std::vector<std::string> Server::parsedInput(std::string str)
{
	std::vector<std::string> ret;

	ret = split(str, '\n');
	if (ret.size() == 0)
		return (ret);
	for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); ++it)
		removeCarriageReturn(*it);
	if (ret[ret.size() - 1][0] == '\0')
		ret.pop_back();
	return(ret);
}

void Server::checkCommand(std::string line, Client &client, t_data &cmd)
{
	std::string command = line.substr(0, line.find(" "));

	if (command == "CAP" || command == "cap")
		return ;
	else if (command == "QUIT" || command == "quit")
		this->quitCommand(line, client, cmd);
	else if (command == "PASS" || command == "pass")
		this->passCommand(line, client, cmd);
	else if (client.hasCorrectPass() == false)
		this->createResponse(ERR_NOTCORRECTPASS, cmd, ONLY_CLIENT);
	else if (command == "NICK" || command == "nick")
		this->nickCommand(line, client, cmd);
	else if (command == "USER" || command == "user")
		this->userCommand(line, client, cmd);
	else if (client.isAuth() == false)
		this->createResponse(ERR_NOTCORRECTNICK, cmd, ONLY_CLIENT);
	else if (command == "JOIN" || command == "join")
		this->joinCommand(line, client, cmd);
	else if (command == "WHO" || command == "who")
		return ;
	else if (command == "KICK" || command == "kick")
		this->kickCommand(line, client, cmd);
	else if (command == "TOPIC" || command == "topic")
		this->topicCommand(line, client, cmd);
	else if (command == "INVITE" || command == "invite")
		this->inviteCommand(line, client, cmd);
	else if (command == "MODE" || command == "mode")
		this->modes(line, client, cmd);
	// else if (command == "PART" || command == "part")
	// 	this->partCommand(line, client, cmd);
	else if (command == "PRIVMSG" || command == "privmsg")
		this->privmsgCommand(line, client, cmd);
	else if (command == "INFO" || command == "info")
		this->infoCommand(cmd);
	else if (command == "BOT" || command == "bot")
		this->botCommand(line, cmd);
	else
		this->createResponse(ERR_UNKNOWNCOMMAND, cmd, ONLY_CLIENT);
}