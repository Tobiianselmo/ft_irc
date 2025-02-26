#include "../include/Server.hpp"

Server::~Server()
{
	if (_serverSocket != -1)
		close(_serverSocket);
}

Server::Server(int port,const std::string &password)
{
	this->_port = port;
	this->_password = password;
	this->_serverSocket = -1;
}

int Server::getPort() const { return this->_port; }

const std::string &Server::getPassword() const { return this->_password; }

Server &Server::operator=(const Server &data)
{
    if (this != &data)
    {
        this->_port = data._port;
	    this->_password = data._password;
	    this->_serverSocket = data._serverSocket;
    }
    return (*this);
}

void Server::setupServer()
{
    // Crear el socket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
		throw std::runtime_error("Error al crear el socket: ");
	
	// Configurar el socket para reutilizar la dirección
	int opt = 1;
	if(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(_serverSocket);
		throw std::runtime_error("Error al configurar el socket: ");
	}
	// Configurar el socket como no bloqueante
	if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		close(_serverSocket);
		throw (std::runtime_error("Failed to set non-blocking mode on server socket"));
	}
	struct sockaddr_in serverAddress;

	std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Escuchar en todas las interfaces
    serverAddress.sin_port = htons(_port); // Puerto en formato de red

	if (bind(_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
	{
        close(_serverSocket);
        throw std::runtime_error("Error al enlazar el socket: ");
    }
	if (listen(_serverSocket, SOMAXCONN) == -1) {
        close(_serverSocket);
        throw std::runtime_error("Error al escuchar en el socket: ");
    }

    std::cout << "Servidor escuchando en el puerto " << _port << std::endl;
}

void Server::handleConnections()
{
    struct pollfd fds[1];
    fds[0].fd = _serverSocket;
    fds[0].events = POLLIN; // Esperar eventos de lectura
    
    Client _client("testnick", "testuser", *this);

    _client.setupClient();
    while (true)
	{
        int ret = poll(fds, 1, -1); // Esperar indefinidamente
        if (ret == -1)
            throw std::runtime_error("Error en poll(): ");

        // Verificar si hay una nueva conexión
        if (fds[0].revents & POLLIN)
		{
            struct sockaddr_in clientAddress;
            socklen_t clientAddressLength = sizeof(clientAddress);
            int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            if (clientSocket == -1)
			{
                std::cerr << "Error al aceptar la conexión: "<< std::endl;
                continue;
            }

            char buffer[1024] = {0};

            memset(buffer, 0, sizeof(buffer));
			int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bytes <= 0)
			{
				std::cout << "desconectado" << std::endl;
				break ;
			}
			std::cout << "Message from client: " << buffer << std::endl;
            
            std::cout << "Nuevo cliente conectado" << std::endl;

            // Aquí puedes agregar el cliente a una lista de clientes conectados
            // y manejar su autenticación y comandos.
        }
    }
}

// int main()
// {
// 	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

// 	// Es el tipo de dato que se utiliza para almacenar la direccion del socket.
// 	sockaddr_in serverAddress;
	
// 	serverAddress.sin_family = AF_INET;
	
// 	// esta función se utiliza para convertir el entero sin signo del
// 	// orden de bytes de la máquina al orden de bytes de la red.
// 	serverAddress.sin_port = htons(8080);
	
// 	// Se utiliza cuando no queremos vincular nuestro socket a ninguna IP en
// 	// particular y en su lugar hacer que escuche todas las IP disponibles.
// 	serverAddress.sin_addr.s_addr = INADDR_ANY;


// 	// Vinculacion del socket del servidor
// 	bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	
// 	listen(serverSocket, 1);

// 	// Aceptar la conexion del cliente
// 	while (true)
// 	{
// 		int clientSocket = accept(serverSocket, nullptr, nullptr);
		
// 		// Recibir datos del cliente
// 		char buffer[1024] = {0};
// 		while (true)
// 		{
// 			memset(buffer, 0, sizeof(buffer));
// 			int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
// 			if (bytes <= 0)
// 			{
// 				std::cout << "desconectado" << std::endl;
// 				break ;
// 			}
// 			std::cout << "Message from client: " << buffer << std::endl;
// 		}
// 		close(clientSocket);
// 	}
// 	// Cerramos el socket del servidor
// 	close(serverSocket);
// 	return (0);
// }