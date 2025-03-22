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
		std::string _nowTime;
		std::string _topicCreate;

		int _users;
		int _usersLimit;

		bool _inviteOnly;
		bool _moderate;
		bool _externMessages;
		bool _hasLimit;
		bool _hasPassword;
		bool _hasTopic;

		std::vector<Client *> _clients;
		std::vector<Client *>	_invites;
		std::vector<Client *> _operators;
	public:
		Channel();
		~Channel();
		Channel(const std::string &name);

		// Setters
		void	setName(std::string name);
		void	setPassword(std::string password);
		void	setHasPassword(bool value);
		void	setHasLimit(bool value);
		void	setLimit(int limit);
		void	setTopic(std::string topic,bool val,std::string topicCreate);
		void	setInviteOnly(bool choice);
		
		// Getters
		std::string getName() const;
		std::string getPassword() const;	
		std::string getTopic() const;
		std::string getTopicCreate() const;
		std::string getNowTime() const;
		bool		hasPassword() const;
		bool		hasTopic() const;
		bool		hasLimit() const;
		bool		getInvite() const;
		void		getMode() const;
		int			getUserSize() const;
		int			getUsersLimit() const;
		Client		*getClient(std::string nick);
		std::vector<Client *> getArrClients() const;

		// Bools
		bool	isClient(const Client &client);
		bool	isClient(const std::string &name);
		bool	isOperator(std::string nick) const;
		bool	isInvited(std::string nick) const;

		// Add
		void	addClient(Client *client);
		void	addOperator(Client *client);
		void	addInvite(Client *client);
		

		// Deleters
		void	deleteClient(Client *client);
		void	deleteOperators(Client *client);
		void	deleteInvited(Client *client);
		void	deletePassword();
		// Prints
		void	printChannel();
		void	printClients();
		void	printInvited();
		void	printOperators();
		void	sendModes(t_data &cmd);
};

#endif