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
		std::vector<struct pollfd>	getFdsVector() const;
		bool				isDuplicated(std::string name);
		Channel				*getChannel(std::string name);
		Client				*getClient(std::string name);

		void	setHostName(std::string hostname);

		void	remClientFromServ(Client &client, int i);

		t_data	initStructure(std::string msg, Client &client);
		
		void	setupServer();
		void	handleConnections();
		void	newConnections();
		void	eventMsg(int i, Client &client);

		void	checkCommand(std::vector<std::string> arr,Client &client, t_data &cmd);
		std::vector<std::string>	parsedInput(std::string str);

		void	createResponse(int err, t_data &cmd, int sendTo);
		void	sendMsgToServer(std::string msg);

		void	passCommand(std::string line, Client &client, t_data &cmd);
		void	nickCommand(std::vector<std::string> arr, Client &client, t_data &cmd);
		void	userCommand(std::string line, Client &client, t_data &cmd);
		void	joinCommand(std::string line, Client &client, t_data &cmd);
		void	kickCommand(std::string line, Client &client, t_data &cmd);
		void	topicCommand(std::string line, Client &client, t_data &cmd);
		void	inviteCommand(std::string line, Client &client, t_data &cmd);
		void	cutCommand(std::string line, Client &client, t_data &cmd);
		void	quitCommand(std::string line, Client &client, t_data &cmd);

		void	modes(std::string &line, Client &client, t_data &cmd);
		void	addMode(std::vector<std::string> &line, Client &client, t_data &cmd);
		void	delMode(std::vector<std::string> &line, Client &client, t_data &cmd);

		void	sendMsgToServer(std::string msg);
	};

#endif