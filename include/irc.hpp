#ifndef IRC_HPP
#define IRC_HPP

# include <algorithm>
# include <iostream>
# include <cstring>
# include <csignal>
# include <cstdlib>
# include <sstream>
# include <cctype>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <stdexcept>
# include <cstring>
# include <poll.h>
# include <vector>
# include <map>
# include <sstream>
# include <ctime>

class Server;
class Channel;
class Client;

typedef struct s_data
{
	std::string	msg; // Complete line
	std::string	destUser; // User who will be affected
	std::string	privMessage; // Message to send in PRIVMSG
	std::string	channelName;
	std::string	authMsg;
	std::string	cmdType; // ie: JOIN, KICK

	std::string	prefix; // Mode prefix
	std::string	suffix; // Mode suffix
	std::string description; // Mode error description

	Client		*client; // Pointer to the client who send the request
	Channel		*channel; // Pointer to the Channel
} t_data;


typedef enum
{
	ALL_CHANNEL,
	ALL_CLIENTS,
	ONLY_OPERATORS,
	ONE_CLIENT,
	NOT_ALL_CHANNEL,
	ONLY_CLIENT
} SEND;

typedef enum
{
	RPL_JOIN,
	RPL_WELCOME,
	RPL_MODE,
	RPL_PART,
	RPL_QUIT,
	RPL_CONTROLC,
	ERR_NOLIMIT,
	ERR_LIMITSET,
	ERR_INVALIDLIMIT,
	RPL_NICKSUCCESS,
	RPL_KICK,
	ERR_INVALIDCHAR,
	RPL_INVITE,
	RPL_TOPIC_mode,
	RPL_KEY,
	RPL_OPERATOR,
	RPL_PRIVMSGSUCCESS,
	RPL_REMOVETOPIC,
	ERR_NOTCORRECTPASS,
	ERR_NOTCORRECTNICK,
	ERR_INVALIDPARAMS,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_INVITELIST = 336,
	RPL_ENDOFINVITELIST = 337,
	RPL_INVITING = 341,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_NOTEXTTOSEND = 412,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_NOOPERHOST = 491,
	ERR_INVALIDKEY = 525,
	ERR_INVALIDMODEPARAM = 696,
} NUMERICS;

// Utils
std::vector<std::string>	split(const std::string &str, char delimiter);
const char					*checkNickName(const char *str);
std::string					intToString(int nbr);
std::string					join(const std::vector<std::string>::iterator &vec, const std::string& delimiter, size_t size);
std::string					commandToUpper(std::string line);

// RESPONSES
void						sendMsgToChannel(std::string msg, int sendTo, t_data &cmd);
std::string					rpl_namreply(Server *server, t_data &cmd, std::string err);

#endif