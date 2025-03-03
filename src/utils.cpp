#include "../include/Server.hpp"

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
