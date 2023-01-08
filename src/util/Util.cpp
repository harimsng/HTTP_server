#include <sstream>

#include "Util.hpp"
#include "Logger.hpp"

using namespace std;

bool
Util::parseArgument(int argc, char **argv)
{
	if (argc == 1)
		return false;
	for (int i = 1; i < argc - 1; ++i)
	{
		string				arg(argv[i]);
		string::size_type	pos;

		if (!(arg[0] == '-' && arg[1] == '-'))
			return false;
		pos = arg.find('=');
		if (arg.substr(2, pos - 2) != "log")
			return false;
		arg = arg.substr(pos + 1, string::npos);
		Logger::initLogger(arg);
	}
	return true;
}

const string
Util::getDate(const char* format)
{
	time_t		curTime;
	struct tm*	curTimeInfo;
	char		timeBuf[1024];

	time(&curTime);
	curTimeInfo = localtime(&curTime);
	strftime(timeBuf, 1024, format, curTimeInfo);
	return (timeBuf);
}

int
Util::toInt(const string& str)
{
	stringstream	ss(str);
	int				num = 0;

	ss >> num;
	return num;
}

string
Util::toString(int num)
{
	string	str;
	int		denom = 1000000000;
	
	while (denom > 0 && num / denom == 0)
		denom /= 10;
	while (denom > 0)
	{
		str.push_back(num / denom + '0');
		num %= denom;
		denom /= 10;
	}
	return str;
}
