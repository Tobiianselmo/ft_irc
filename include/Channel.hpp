#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class Client;

typedef enum channelType
{
	REGULAR_CH,
	LOCAL_CH
};

class Channel
{
	private:
		std::string _name;
		std::string _password;

		bool _private;

		std::vector<Client *> _clients;

		channelType _type;
	public:
		Channel();
		~Channel();
};

#endif