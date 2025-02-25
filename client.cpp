#include "include/client.hpp"

int main()
{
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	// Enviar la informacion
	while (true) {
        std::string message;
        std::getline(std::cin, message);

        if (message == "exit")
		{
            break ;
        }

        send(clientSocket, message.c_str(), message.size(), 0);
    }
	// const char * message = "Hello, server!";
	// send(clientSocket, message, strlen(message), 0);

	close(clientSocket);
	return (0);
}