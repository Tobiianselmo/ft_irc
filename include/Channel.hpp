#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include <set>

class Client;

class Channel
{
	private:
		std::string _name;
		std::string _password;
		std::string _topic;

		int _users;
		int _usersLimit;

		bool _inviteOnly;
		bool _moderate;
		bool _externMessages;
		bool _hasLimit;
		bool _hasPassword;

		std::vector<Client>	_invites;
		std::vector<Client> _clients;
		std::vector<Client> _operators;
	public:
		Channel();
		~Channel();
		Channel(const std::string &name);

		void	setName(std::string name);
		void	setPassword(std::string password);
		void	setTopic(std::string topic);
		void	setMode(std::string mode, bool choice);
		
		std::string getName() const;
		std::string getPassword() const;	
		std::string getTopic() const;
		bool		getInvite() const;
		void		getMode() const;
		Client		*getClients(std::string nick);

		bool	isClient(const Client &client);
		void	addClient(const Client &client);
		bool	isOperator(std::string nick) const;
		void	addOperator(const Client &client);
		void	deleteOperators(const Client &client);
		
		void	printClients();
		void	addInvite(const Client &client);
		bool	isInvited(std::string nick) const;
		void	printInvited();
		void	printOperators();
};

#endif