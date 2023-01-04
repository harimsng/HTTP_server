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
