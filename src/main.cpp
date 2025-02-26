#include "../include/Server.hpp"

int validPort(char *str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        if(!isdigit(str[i]))
            return(0);
    }
    if (std::atoi(str) < 1024 || std::atoi(str) > 65535)
        return(0);
    return(1);
}

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        std::cout << "Error: You must use ./ircserv <port> <password>" << std::endl;
        return(1);
    }
    if(!validPort(argv[1]))
    {
        std::cout << "Error: Introduce number between 1024 and 65535" << std::endl;
        return(1);
    }
	Server server(std::atoi(argv[1]),argv[2]);
	try
	{
		server.setupServer();
		server.handleConnections();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
