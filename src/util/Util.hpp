#ifndef Util_hpp
#define Util_hpp

#include <string>

#include "socket/Socket.hpp"

class Util
{
public:
	static uint64_t			convertAddrKey(uint32_t addr, uint16_t port);
	static void				convertAddrKey(uint64_t addrKey, uint32_t& addr, uint16_t& port);

	static bool					parseArgument(int argc, char **argv);
	static std::string			getFormattedAddress(uint32_t addr, uint16_t port);
	static std::string			getFormattedAddress(sockaddr_in& addr);
	static std::string			getFormattedAddress(int fd);

	static int					toInt(const std::string& str);
	static std::string			toString(int num);
	static std::string			toUpper(std::string str);

	static const std::string	getDate(const char* format);
	static bool					checkFileStat(const char* path);

};
#endif //Util_hpp
