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

		std::vector<Client> _clients;
		std::vector<Client> _operators;
	public:
		Channel();
		~Channel();
		Channel(const std::string &name);

		void	setName(std::string name);
		void	setPassword(std::string password);
		void	setTopic(std::string topic);
		
		std::string getName() const;
		std::string getPassword() const;	
		std::string getTopic() const;

		bool	isClient(const Client &client);
		void	addClient(const Client &client);
		void	addOperator(const Client &client);
};

#endif