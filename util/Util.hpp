#ifndef Util_hpp
#define Util_hpp

#include <string>

class Util
{
public:
	static const std::string getDate(const char* format);
	static bool	parseArgument(int argc, char **argv);

};
#endif //Util_hpp
