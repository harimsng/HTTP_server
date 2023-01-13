#ifndef Util_hpp
#define Util_hpp

#include <string>

class Util
{
public:
	static bool					parseArgument(int argc, char **argv);
	static int					toInt(const std::string& str);
	static std::string			toString(int num);
	static const std::string	getDate(const char* format);
	static bool					checkFileStat(const char* path);

};
#endif //Util_hpp
