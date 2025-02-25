#include "../include/server.hpp"

int main()
{
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	// Es el tipo de dato que se utiliza para almacenar la direccion del socket.
	sockaddr_in serverAddress;
	
	serverAddress.sin_family = AF_INET;
	
	// esta función se utiliza para convertir el entero sin signo del
	// orden de bytes de la máquina al orden de bytes de la red.
	serverAddress.sin_port = htons(8080);
	
	// Se utiliza cuando no queremos vincular nuestro socket a ninguna IP en
	// particular y en su lugar hacer que escuche todas las IP disponibles.
	serverAddress.sin_addr.s_addr = INADDR_ANY;


	// Vinculacion del socket del servidor
	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	
	listen(serverSocket, 1);

	// Aceptar la conexion del cliente
	while (true)
	{
		int clientSocket = accept(serverSocket, nullptr, nullptr);
		
		// Recibir datos del cliente
		char buffer[1024] = {0};
		while (true)
		{
			memset(buffer, 0, sizeof(buffer));
			int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bytes <= 0)
			{
				std::cout << "desconectado" << std::endl;
				break ;
			}
			std::cout << "Message from client: " << buffer << std::endl;
		}
		close(clientSocket);
	}
	// Cerramos el socket del servidor
	close(serverSocket);
	return (0);
}