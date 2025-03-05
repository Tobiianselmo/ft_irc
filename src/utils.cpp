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
	return tokens;
}

void    checkNickName(std::string str)
{
	if (!std::strchr("$:#&", str[0]) || std::isdigit(str[0])) // If NickName begins with "$:#&"
	{
		std::cout << "Error " << ERR_ERRONEUSNICKNAME << std::endl; 
		return ;
	}
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!std::strchr(" ,*?!@.", str[i]))
		{
			std::cout << "Error " << ERR_ERRONEUSNICKNAME << std::endl; 
			return ;
		}
	}
}