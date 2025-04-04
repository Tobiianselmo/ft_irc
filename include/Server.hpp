#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Client;
class Channel;

extern	int g_global;

class Server
{
	private:
		int							_port;
		int							_serverSocket;
		std::string					_password;

		int							_signal;
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
		int					getChannelsSize() const;
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
		static void	handleSignal(int signal);
		void	newConnections();
		void	eventMsg(int i, Client &client);
		
		void	checkCommand(std::string line, Client &client, t_data &cmd);
		std::vector<std::string>	parsedInput(std::string str);
		
		void	createResponse(int err, t_data &cmd, int sendTo);
		void	sendMsgToServer(std::string msg);
		void	sendMsgToOneClient(std::string msg, t_data &cmd);
		void	sendMsgToAllChannels(const Client  & client, t_data &cmd);

		void	passCommand(std::string line, Client &client, t_data &cmd);
		void	nickCommand(std::string line, Client &client, t_data &cmd);
		void	userCommand(std::string line, Client &client, t_data &cmd);
		void	joinCommand(std::string line, Client &client, t_data &cmd);
		void	kickCommand(std::string line, Client &client, t_data &cmd);
		void	topicCommand(std::string line, Client &client, t_data &cmd);
		void	inviteCommand(std::string line, Client &client, t_data &cmd);
		void	partCommand(std::string line, Client &client, t_data &cmd);
		void	quitCommand(std::string line, Client &client, t_data &cmd);
		void	privmsgCommand(std::string line, Client &client, t_data &cmd);
		void	infoCommand(t_data &cmd);
		void	botCommand(std::string line, t_data &cmd);

		void	modeCommand(std::string &line, Client &client, t_data &cmd);
		int		addMode(std::vector<std::string> &line, Client &client, t_data &cmd, int *i);
		int		delMode(std::vector<std::string> &line, Client &client, t_data &cmd, int *i);

		int		addChannelOperator(t_data &cmd, std::vector<std::string> &line, int args);
		int		addChannelLimit(t_data &cmd, std::vector<std::string> &line, int args);
		int		addChannelKey(t_data &cmd, std::vector<std::string> &line, int args);

		int		delChannelOperator(t_data &cmd, std::vector<std::string> &line, int args);
		int		delChannelKey(t_data &cmd, std::vector<std::string> &line, int args);
		int		delChannelLimit(t_data &cmd, std::vector<std::string> &line, int args);
};

#endif