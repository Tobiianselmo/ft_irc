#include "../include/Client.hpp"

Client::~Client()
{
	delete _server;
}

Client::Client(const std::string &nick, const std::string &user, const Server &server)
{
	this->_nickName = nick;
	this->_userName = user;
	_server = new Server(server.getPort(), server.getPassword());
}

void Client::setupClient()
{
	_clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_clientSocket == -1)
		throw std::runtime_error("Error al crear el socket: ");

	sockaddr_in serverAddress;

	std::memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(_server->getPort());
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	connect(_clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	// Enviar la informacion
	while (true) {
        std::string message;
        std::getline(std::cin, message);

        if (message == "exit")
		{
            break ;
        }

        send(_clientSocket, message.c_str(), message.size(), 0);
    }
	// const char * message = "Hello, server!";
	// send(_clientSocket, message, strlen(message), 0);

	close(_clientSocket);
}

// int main()
// {
// 	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

// 	sockaddr_in serverAddress;
// 	serverAddress.sin_family = AF_INET;
// 	serverAddress.sin_port = htons(8080);
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;

// 	connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

// 	// Enviar la informacion
// 	while (true) {
//         std::string message;
//         std::getline(std::cin, message);

//         if (message == "exit")
// 		{
//             break ;
//         }

//         send(clientSocket, message.c_str(), message.size(), 0);
//     }
// 	// const char * message = "Hello, server!";
// 	// send(clientSocket, message, strlen(message), 0);

// 	close(clientSocket);
// 	return (0);
// }