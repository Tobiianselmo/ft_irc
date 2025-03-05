#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Client;

class Channel;

class Server
{
	private:
		int _port;
		int _serverSocket;
		std::string _password;

		std::vector<struct pollfd >_fds;
		std::vector<Client> _client;
		std::vector<Channel> _channels;

		sockaddr_in _serverAddress;

		std::map<NUMERICS, std::string> _errors;
	public:
		~Server();
		Server(int port, const std::string &password);

		int	getPort() const;
		int	getServerSocket() const;
		const std::string &getPassword() const;

		void	setupServer();
		void	handleConnections();
		void	newConnections();
		void 	eventMsg(std::vector<struct pollfd> &fds, int i);

		void	parsedInput(std::string str);
		void	checkCommand(std::vector<std::string> arr);
		int		joinCommand(std::vector<std::string> arr);
};
std::vector<std::string>	split(const std::string &str, char delimiter);
void						removeCarriageReturn(std::string &str);
void						checkNick(std::string str);
#endif