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
		int							_port;
		int							_serverSocket;
		std::string					_password;

		std::string					_hostName;

		std::vector<struct pollfd>	_fds;
		std::vector<Channel>		_channels;
		std::map<int, Client *>		_clientsMap;

		sockaddr_in					_serverAddress;
	public:
		~Server();
		Server(int port, const std::string &password);

		// Getters
		int					getPort() const;
		int					getServerSocket() const;
		const std::string	&getPassword() const;
		const std::string	&getHostName() const;
		bool				isDuplicated(std::string name);
		Channel				*getChannel(std::string name);
		Client				*getClient(std::string name);

		void	setHostName(std::string hostname);
		
		t_data	initStructure(std::string msg, Client &client);
		
		void	setupServer();
		void	handleConnections();
		void	newConnections();
		void	eventMsg(std::vector<struct pollfd> &fds, int i, Client &client);

		void	checkCommand(std::vector<std::string> arr,Client &client, t_data &cmd);
		std::vector<std::string>	parsedInput(std::string str);

		void	createResponse(int err, t_data &cmd);

		void	passCommand(std::string line, Client &client, t_data &cmd);
		void	nickCommand(std::vector<std::string> arr, Client &client, t_data &cmd);
		void	userCommand(std::string line, Client &client, t_data &cmd);
		void	joinCommand(std::string line, Client &client, t_data &cmd);
		int		topicCommand(std::string str, Client &client,t_data &cmd);
		int		kickCommand(std::string str, Client &client, t_data &cmd);
		int		inviteCommand(std::string line, Client &client);//delfi

		int		modes(std::string &line, Client &client);//delif
		int		addMode(std::vector<std::string> &line, Client &client);//delfi
		int		delMode(std::vector<std::string> &line, Client &client);//delfi
	};

#endif