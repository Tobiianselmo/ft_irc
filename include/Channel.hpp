#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

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
		bool _hasTopic;

		std::vector<Client>	_invites;
		std::vector<Client> _clients;
		std::vector<Client> _operators;
	public:
		Channel();
		~Channel();
		Channel(const std::string &name);

		// Setters
		void	setName(std::string name);
		void	setPassword(std::string password);
		void	setHasPassword(bool value);
		void	setTopic(std::string topic,bool val);
		void	setMode(std::string mode, bool choice);
		
		// Getters
		std::string getName() const;
		std::string getPassword() const;	
		std::string getTopic() const;
		bool		hasPassword() const;
		bool		hasTopic() const;
		bool		getInvite() const;
		void		getMode() const;
		Client		*getClient(std::string nick);
		std::vector<Client> getArrClients() const;

		// Bools
		bool	isClient(const Client &client);
		bool	isClient(const std::string &name);
		bool	isOperator(std::string nick) const;
		bool	isInvited(std::string nick) const;

		// Add
		void	addClient(const Client &client);
		void	addOperator(const Client &client);
		void	addInvite(const Client &client);

		// Deleters
		void	deleteClient(const Client &client);
		void	deleteOperators(const Client &client);

		// Prints
		void	printChannel();
		void	printClients();
		void	printInvited();
		void	printOperators();
};

#endif