#ifndef IRC_HPP
#define IRC_HPP

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <cctype>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cstring>
#include <poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <ctime>

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

	std::string	prefix;
	std::string	suffix;

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
	RPL_CUT,
	RPL_QUIT,
	ERR_NOLIMIT,
	ERR_LIMITSET,
	ERR_INVALIDLIMIT,
	RPL_NICKSUCCESS,
	RPL_KICK,
	ERR_INVALIDCHAR,
	RPL_INVITE,
	RPL_TOPIC_mode,
	RPL_KEY,
	RPL_PRIVMSGSUCCESS,
	RPL_REMOVETOPIC,
	ERR_NOTCORRECTPASS,
	ERR_NOTCORRECTNICK,
	// RPL_YOURHOST = 002,
	// RPL_CREATED = 003,
	// RPL_MYINFO = 004,
	// RPL_ISUPPORT = 005,
	// RPL_BOUNCE = 010,
	// RPL_STATSCOMMANDS = 212,
	// RPL_ENDOFSTATS = 219,
	// RPL_UMODEIS = 221,
	// RPL_STATSUPTIME = 242,
	// RPL_LUSERCLIENT = 251,
	// RPL_LUSEROP = 252,
	// RPL_LUSERUNKNOWN = 253,
	// RPL_LUSERCHANNELS = 254,
	// RPL_LUSERME = 255,
	// RPL_ADMINME = 256,
	// RPL_ADMINLOC1 = 257,
	// RPL_ADMINLOC2 = 258,
	// RPL_ADMINEMAIL = 259,
	// RPL_TRYAGAIN = 263,
	// RPL_LOCALUSERS = 265,
	// RPL_GLOBALUSERS = 266,
	// RPL_WHOISCERTFP = 276,
	// RPL_NONE = 300,
	// RPL_AWAY = 301,
	// RPL_USERHOST = 302,
	// RPL_UNAWAY = 305,
	// RPL_NOWAWAY = 306,
	// RPL_WHOISREGNICK = 307,
	// RPL_WHOISUSER = 311,
	// RPL_WHOISSERVER = 312,
	// RPL_WHOISOPERATOR = 313,
	// RPL_WHOWASUSER = 314,
	// RPL_ENDOFWHO = 315,
	// RPL_WHOISIDLE = 317,
	// RPL_ENDOFWHOIS = 318,
	// RPL_WHOISCHANNELS = 319,
	// RPL_WHOISSPECIAL = 320,
	// RPL_LISTSTART = 321,
	// RPL_LIST = 322,
	// RPL_LISTEND = 323,
	// RPL_CHANNELMODEIS = 324,
	// RPL_CREATIONTIME = 329,
	// RPL_WHOISACCOUNT = 330,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_INVITELIST = 336,
	RPL_ENDOFINVITELIST = 337,
	// RPL_WHOISACTUALLY = 338,
	RPL_INVITING = 341,
	// RPL_INVEXLIST = 346,
	// RPL_ENDOFINVEXLIST = 347,
	// RPL_EXCEPTLIST = 348,
	// RPL_ENDOFEXCEPTLIST = 349,
	// RPL_VERSION = 351,
	// RPL_WHOREPLY = 352,
	RPL_NAMREPLY = 353,
	// RPL_LINKS = 364,
	// RPL_ENDOFLINKS = 365,
	RPL_ENDOFNAMES = 366,
	// RPL_BANLIST = 367,
	// RPL_ENDOFBANLIST = 368,
	// RPL_ENDOFWHOWAS = 369,
	// RPL_INFO = 371,
	// RPL_MOTD = 372,
	// RPL_ENDOFINFO = 374,
	// RPL_MOTDSTART = 375,
	// RPL_ENDOFMOTD = 376,
	// RPL_WHOISHOST = 378,
	// RPL_WHOISMODES = 379,
	// RPL_YOUREOPER = 381,
	// RPL_REHASHING = 382,
	// RPL_TIME = 391,
	// ERR_UNKNOWNERROR = 400,
	ERR_NOSUCHNICK = 401,
	// ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	// ERR_TOOMANYCHANNELS = 405,
	// ERR_WASNOSUCHNICK = 406,
	// ERR_NOORIGIN = 409,
	// ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	// ERR_INPUTTOOLONG = 417,
	ERR_UNKNOWNCOMMAND = 421,
	// ERR_NOMOTD = 422,
	// ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	// ERR_NICKCOLLISION = 436,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	// ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_KEYSET = 467,
	// ERR_YOUREBANNEDCREEP = 465,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	// ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	// ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVSNEEDED = 482,
	// ERR_CANTKILLSERVER = 483,
	ERR_NOOPERHOST = 491,
	// ERR_UMODEUNKNOWNFLAG = 501,
	// ERR_USERSDONTMATCH = 502,
	// ERR_HELPNOTFOUND = 524,
	// ERR_INVALIDKEY = 525,
	// RPL_STARTTLS = 670,
	// RPL_WHOISSECURE = 671,
	// ERR_STARTTLS = 691,
	ERR_INVALIDMODEPARAM = 696,
	// RPL_HELPSTART = 704,
	// RPL_HELPTXT = 705,
	// RPL_ENDOFHELP = 706,
	// ERR_NOPRIVS = 723,
	// RPL_LOGGEDIN = 900,
	// RPL_LOGGEDOUT = 901,
	// ERR_NICKLOCKED = 902,
	// RPL_SASLSUCCESS = 903,
	// ERR_SASLFAIL = 904,
	// ERR_SASLTOOLONG = 905,
	// ERR_SASLABORTED = 906,
	// ERR_SASLALREADY = 907,
	// RPL_SASLMECHS = 908,
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