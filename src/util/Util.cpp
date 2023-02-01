#include <sys/stat.h>

#include <sstream>
#include <cctype>

#include "Logger.hpp"
#include "Util.hpp"

using namespace std;

bool
Util::parseArgument(int argc, char **argv)
{
	string	logLevel = "INFO";

	if (argc == 1)
	{
		return false;
	}
	for (int i = 1; i < argc - 1; ++i)
	{
		string				arg(argv[i]);
		string::size_type	pos;

		if (!(arg[0] == '-' && arg[1] == '-'))
			// TODO: argument error handling
			return false;
		pos = arg.find('=');
		if (arg.substr(2, pos - 2) != "log")
			// error
			return false;
		logLevel = arg.substr(pos + 1, string::npos);
	}
	Logger::initLogger(logLevel);
	return true;
}

const string
Util::getDate(const char* format)
{
	time_t		curTime;
	struct tm*	curTimeInfo;
	char		timeBuf[1024] = {0, };

	time(&curTime);
	curTimeInfo = localtime(&curTime);
	strftime(timeBuf, 1023, format, curTimeInfo);
	return (timeBuf);
}

int
Util::toInt(const string& str)
{
	stringstream	ss(str);
	int				num = 0;

	// TODO: interpret 1m2k3 as 1002003
	ss >> num;
	return num;
}

string
Util::toString(int num)
{
	string	str;
	int		denom = 1000000000;

	if (num == 0)
		return ("0");
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

string
Util::toUpper(string str)
{
	for (string::size_type i = 0; i < str.size(); ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] ^= 32;
	}
	return str;
}

string
Util::toLower(string str)
{
	for (string::size_type i = 0; i < str.size(); ++i)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] ^= 32;
	}
	return str;
}

// TODO: extend and throw
bool
Util::checkFileStat(const char* path)
{
	struct stat buffer;

	if (stat(path, &buffer) == -1)
		return (false);
	return ((buffer.st_mode & S_IFREG) == S_IFREG);
}

