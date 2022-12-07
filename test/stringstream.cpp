#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

std::vector<std::string> splitString(const std::string& str)
{
    std::vector<std::string> tokens;

    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, '\r')) {
        tokens.push_back(token);
    }
	std::cout << ss.fail() << std::endl;
	std::cout << ss.eof() << std::endl;
    return tokens;
}

int main()
{
    std::string str = "C   C\r\nC++\r\nJ\r\n";

    std::vector<std::string> tokens = splitString(str);

    for (auto const &token: tokens)
	{
		if (token == "\n")
			// std::cout << "newline" << std::endl;
			std::cout << (int)token[0] << std::endl;
		else
		{
			for (auto const &tmpChar: token)
				std::cout << (int)tmpChar << " ";
			std::cout << std::endl;
		}

    }

    return 0;
}

