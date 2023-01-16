#ifndef Util_hpp
#define Util_hpp

#include <netinet/in.h>
#include <string>

#include "OsDependency.hpp"

class Util
{
public:
	static bool					parseArgument(int argc, char **argv);
	static std::string			getFormattedAddress(uint32_t addr, uint16_t port);
	static std::string			getFormattedAddress(sockaddr_in& addr);
	static int					toInt(const std::string& str);
	static std::string			toString(int num);
	static const std::string	getDate(const char* format);
	static bool					checkFileStat(const char* path);

};
#endif //Util_hpp
