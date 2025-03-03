#include "../include/Server.hpp"

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = str.find(delimiter);

    while (end != std::string::npos)
	{
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

void    checkNick(std::string str)
{

    if(str[0] == '$' || str[0] == ':' || str[0] == '#' || str[0] == '&')
    {
        std::cout << "error\n";
        return;
    }
    if( std::strchr(str.c_str(),' ')    || std::strchr(str.c_str(),',')
        || std::strchr(str.c_str(),'*') || std::strchr(str.c_str(),'?')
        || std::strchr(str.c_str(),'!') || std::strchr(str.c_str(),'@')
        || std::strchr(str.c_str(),'.'))
    {
        std::cout << "error\n";
        return;
    }
}
