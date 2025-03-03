#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Client;

// typedef enum channelType
// {
// 	REGULAR_CH,
// 	LOCAL_CH
// };

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

		std::vector<Client *> _clients;

		// channelType _type;
	public:
		Channel();
		~Channel();
		Channel &operator=(const Channel &other);
		Channel(const Channel &other);
};

#endif