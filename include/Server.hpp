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

		std::string _hostName;

		std::vector<struct pollfd >_fds;
		// std::vector<Client> _client;
		std::vector<Channel> _channels;
		std::map<int, Client *> _clientsMap;

		sockaddr_in _serverAddress;
	public:
		~Server();
		Server(int port, const std::string &password);

		int	getPort() const;
		int	getServerSocket() const;
		const std::string	&getPassword() const;
		const std::string	&getHostName() const;
		bool	isDuplicated(std::string name);
		Channel	*getChannel(std::string name);
		Client	*getClient(std::string nick);//delfi

		void	createResponse(int err, Client &client, const std::string &channelName);

		void	setupServer();
		void	setHostName(std::string hostname);
		void	handleConnections();
		void	newConnections();
		void 	eventMsg(std::vector<struct pollfd> &fds, int i, Client &client);
		void	authClient(std::vector<std::string> arr, Client &client);
		void	checkCommand(std::vector<std::string> arr,Client &client);
		std::vector<std::string>	parsedInput(std::string str);
		int		kickCommand(std::string str,Client &client);
		int		topicCommand(std::string str,Client &client);

		int		joinCommand(std::string line, Client &client);
		int		inviteCommand(std::string line, Client &client);//delfi

		int		modes(std::string &line, Client &client);//delif
		int		addMode(std::vector<std::string> &line, Client &client);//delfi
		int		delMode(std::vector<std::string> &line, Client &client);//delfi
};

#endif