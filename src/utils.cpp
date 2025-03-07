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

std::string checkNickName(std::string str)
{
	if (std::strchr("$:#&", str[0]) || std::isdigit(str[0]))
	{
		std::cout << "Error " << ERR_ERRONEUSNICKNAME << std::endl; 
		return NULL;
	}
	for (size_t i = 0; i < str.size(); i++)
	{
		if (std::strchr(" ,*?!@.", str[i]))
		{
			std::cout << "Error " << ERR_ERRONEUSNICKNAME << std::endl;
			return NULL;
		}
	}
	return(str);
}