#include "../include/irc.hpp"

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

	while (end != std::string::npos)
	{
		if (start != end)
			tokens.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(delimiter, start);
	}
	if (start < str.size())
		tokens.push_back(str.substr(start));
	return tokens;
}

const char *checkNickName(const char *str)
{
	if (std::strchr("$:#&", str[0]) || std::isdigit(str[0]))
		return NULL;
	for (size_t i = 0; i < std::strlen(str); i++)
	{
		if (std::strchr(" ,*?!@.", str[i]))
			return NULL;
	}
	return(str);
}

std::string intToString(int nbr)
{
	std::ostringstream ss;
	ss << nbr;
	return ss.str();
}

std::string join(const std::vector<std::string>::iterator &vec, const std::string &delimiter, size_t size)
{
	std::string result;
	size_t totalSize = 0;
	for (size_t i = 0; i < size; ++i)
		totalSize += vec[i].size();
	totalSize += delimiter.size() * (size - 1);
	result.reserve(totalSize);

	for (size_t i = 0; i < size; ++i)
	{
		result += vec[i];
		if (i != size - 1)
			result += delimiter;
	}
	return result;
}

std::string join(const std::vector<std::string> &vec)
{
	std::string result;

	if (vec.size() > 0)
	{
		for (size_t i = 0; i < vec.size(); i++)
			result += vec[i] + " "; 
		result.erase(result.end() - 1);
	}
	return result;
}

std::string commandToUpper(std::string line)
{
	std::vector<std::string> splitParams = split(line, ' ');
	
	for (size_t i = 0; i < splitParams[0].size(); i++)
		splitParams[0][i] = toupper(splitParams[0][i]);

	std::string ret = join(splitParams);
	return ret;
}