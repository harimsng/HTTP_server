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
		if (arg.substr(2, pos - 2) == "log")
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
Util::hexToDecimal(const std::string& str)
{
	int decimal = 0;

	if (str == "")
		return (-1);
	for (size_t i = 0; i < str.size(); i++)
	{
		decimal *= 16;
		if (std::isdigit(str[i]))
			decimal += str[i] - '0';
		else
			decimal += str[i] - 'a' + 10;
	}
	return (decimal);
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

string
Util::fixDoublelashes(const string& path, const string& file)
{
    string newStr = path;

	if (*(path.end() - 1) == '/' && *(file.begin()) == '/')
	{
		newStr = path.substr(0, path.find_last_of('/')) + file;
	}
	else if (*(path.end() - 1) != '/' && *(file.begin()) != '/')
	{
	newStr = path + "/" + file;
	}
	else
	{
		newStr = path + file;
	}
	return newStr;
}

string
Util::toHex(unsigned int num)
{
	stringstream	ss;

	ss.flags(ios_base::hex);
	ss << num;
	return ss.str();
}
