#include "../include/irc.hpp"

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = str.find(delimiter);

	if (end != std::string::npos)
	{
		while (end != std::string::npos)
		{
			tokens.push_back(str.substr(start, end - start));
			start = end + 1;
			end = str.find(delimiter, start);
		}
		tokens.push_back(str.substr(start));
	}
	else
		tokens.push_back(str);
	return tokens;
}

const char *checkNickName(const char *str)
{
	if (std::strchr("$:#&", str[0]) || std::isdigit(str[0]))
	{
		std::cout << "Error " << ERR_ERRONEUSNICKNAME << std::endl; 
		return NULL;
	}
	for (size_t i = 0; i < std::strlen(str); i++)
	{
		if (std::strchr(" ,*?!@.", str[i]))
		{
			std::cout << "Error " << ERR_ERRONEUSNICKNAME << std::endl;
			return NULL;
		}
	}
	return(str);
}

std::string join(const std::vector<std::string>::iterator &vec, const std::string &delimiter,size_t size)
{
    std::string result;
    size_t totalSize = 0;
    for (size_t i = 0; i < size; ++i)
	{
		totalSize += vec[i].size();
		std::cout << vec[i] << std::endl;
	}
    totalSize += delimiter.size() * (size - 1); // Tamaño de los delimitadores
    result.reserve(totalSize); // Reservar espacio

    for (size_t i = 0; i < size; ++i)
	{
        result += vec[i];
        if (i != size - 1)
            result += delimiter;
    }
    return result;
}

std::string makeString(std::string channel,std::string username,std::string str,int err,std::string client)
{
	std::string ret;
	switch (err)
	{
		case ERR_NOSUCHCHANNEL:
			ret = username + " " + channel + " :" + str;
			return(ret);
		case ERR_CHANOPRIVSNEEDED:
			ret = username + " " + channel + " :" + str;
			return(ret);
		case ERR_NOTONCHANNEL:
			ret = username + " " + channel + " :" + str;
			return(ret);
		case ERR_USERNOTINCHANNEL:
			ret = username + " " +  client + " " + channel + " :" + str;
	}
	return(ret);
}