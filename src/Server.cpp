#include "../include/Server.hpp"

Server::Server(){}

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
    struct pollfd fd;
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
    fd.fd = _serverSocket;
    fd.events = POLLIN;
    fd.revents = 0;
    _fds.push_back(fd);
}

void Server::handleConnections()
{
   // int i = 0;
    /*         Client _client("testnick", "testuser", *this);
        
            _client.setupClient(); */
    while (true)
	{
        int ret = poll(&_fds[0], _fds.size(), -1); // Esperar indefinidamente
        if (ret == -1)
            throw std::runtime_error("Error en poll(): ");
        
        // Verificar si hay una nueva conexión
        for(size_t i = 0; i < _fds.size() ; i++)
        {
            if (_fds[i].revents & POLLIN) //GESTIONO LOS EVENTOS
            {
                if(_fds[i].fd == _serverSocket) // ES UN EVENTO DEL SERVIDOR OSEA I == 0 SE TRATA DE UNA NUEVA CONEXION
                    newConnections();
                else
                    eventMsg(_fds, i);
               
    
                // Aquí puedes agregar el cliente a una lista de clientes conectados
                // y manejar su autenticación y comandos.
            }
        }
    }
}

void Server::newConnections()
{
    struct pollfd newuser;
    struct sockaddr_in clientAddress;
    int error;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1)
        std::cerr << "Error al aceptar la conexión: "<< std::endl;
    error = fcntl(clientSocket, F_GETFL, 0);
	if (error == -1 || fcntl(clientSocket, F_SETFL, error | O_NONBLOCK) == -1)
	{
		//perror("fcntl");
		close(clientSocket);
		return;
	}
    std::cout << "Nuevo cliente conectado" << std::endl;
    newuser.fd = clientSocket;
    Client *user = new Client(clientSocket);
    newuser.events = POLLIN | POLLOUT;
    newuser.revents = 0;
    _fds.push_back(newuser);
    _client.push_back(user);
    // send(clientSocket, , message.size(), 0);
}
    
void Server::eventMsg(std::vector<struct pollfd> &fds, int i)
{
    int clientSocket = fds[i].fd;
    char buffer[1024] = {0};
    
    std::memset(buffer, 0, sizeof(buffer));
    int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytes <= 0)
    {
        close (fds[i].fd);
        fds.erase(fds.begin() + i);
        std::cout << "desconectado" << std::endl;
    }
    this->parsedInput(buffer);
}

void Server::parsedInput(std::string str)
{
    std::vector<std::string> ret;
    ret = split(str,'\n');
    for (std::vector<std::string>::iterator it = ret.begin(); it != ret.end(); ++it)
        removeCarriageReturn(*it);
    ret.pop_back(); //deja un NULL al final por eso lo borramos
    this->checkCommand(ret);
}
void removeCarriageReturn(std::string &str)
{
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
}
void Server::checkCommand(std::vector<std::string> arr)
{
	std::vector<std::string> aux;
    if(!std::strncmp(arr[0].c_str(),"PASS ",5))
    {
        if(arr[0].c_str() + 5 != this->getPassword())
        std::cout << "ERR_PASSWDMISMATCH (464): Password incorrect" << std::endl;
        return ;
    }
	if(arr.size() > 1) // First input with client data.
	{
		size_t j = 0;
		size_t i = 0;
        if(!std::strncmp(arr[0].c_str(),"NICK ",5))
            checkNick(arr[0].c_str() + 5);
		while (i < arr.size())
		{
			aux = split(arr[i], ' ');
			while (j < aux.size())
			{
				if (aux[0] == "NICK")
					
				j++;
			}
			i++;
		}
	}
	else
	{
		aux = split(arr[0],' ');
		if (aux[0] == "JOIN")
		{
			std::cout << "Entra al JOIN\n";
		}
		else if (aux[0] == "MODE")
		{
			std::cout << "Entra al MODE\n";
		}
/* 		else
			std::cout << "Command not valid." << std::endl; */
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