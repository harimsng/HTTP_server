#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

#include "Types.hpp"

struct sockaddr_in;

class Util
{
public:
	static bool					parseArgument(int argc, char **argv);

	static int					hexToDecimal(const std::string& str);
	static int					toInt(const std::string& str);
	static std::string			toString(int num);
	static std::string			toUpper(std::string str);
	static std::string			toLower(std::string str);
	static std::string			toHex(unsigned int num);

	static const std::string	getDate(const char* format);
	static bool					checkFileStat(const char* path);
	static std::string			fixDoublelashes(const std::string& path, const std::string& file);
};
#endif //Util_hpp
