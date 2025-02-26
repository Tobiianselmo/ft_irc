#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>

class Server
{
	private:
			int _port;
			std::string _password;
			Server();
			Server(const Server &data);
			Server &operator=(const Server &data);
	public:
			Server(int port,const std::string &password);
			void initServer();
			~Server();
};
#endif