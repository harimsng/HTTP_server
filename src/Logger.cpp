#include <cstdio>

#include "ServerManager.hpp"
#include "Logger.hpp"

#define	MAX_BUFFER_LEN (128)

using namespace std;

const char*	Logger::s_prefixTable[] = {
	"[INFO]    ",
	"[WARNING] ",
	"[ERROR]   ",
	"[DEBUG]   ",
};

// const string
// Util::getDate(const char* format);

Logger::e_types		Logger::s_type = ERROR;
ostream*			Logger::s_ostream = NULL;

// constructors & destructor
Logger::Logger()
{
}

Logger::~Logger()
{
}

Logger::Logger(Logger const& logger)
{
	throw logger;
}

// operators
Logger&
Logger::operator=(Logger const& logger)
{
	throw logger;
	return *this;
}

void
Logger::initLogger(const std::string& type, std::ostream& os)
{
	int	temp;

	s_ostream = &os;
	temp = DISABLED
		+ INFO * (type == "INFO")
		+ WARNING * (type == "WARNING")
		+ ERROR * (type == "ERROR")
		+ DEBUG * (type == "DEBUG");
	s_type = static_cast<e_types>(temp);
	cout << s_prefixTable[INFO]
		<< "vsnprintf() in Logger::log(e_Types, const char*, ...) is C99 feature. should be removed later.\n";
}

void
Logger::log(e_types type, const char* format, ...)
{
	std::string	prefix;
	std::string	suffix;
	char		buffer[MAX_BUFFER_LEN] = {0, };
	va_list		ap;

	if (s_type == DISABLED || s_type < type)
		return;
	va_start(ap, format);
	vsnprintf(buffer, MAX_BUFFER_LEN - 1, format, ap);
	prefix = s_prefixTable[type - 1];
	if (type == ERROR)
	{
		suffix = " (" + Util::toString(errno) + " "
			+ std::strerror(errno) + ")";
	}
	prefix.append(Util::getDate("%F %T "));
	*s_ostream << prefix << buffer << suffix << '\n';
}
