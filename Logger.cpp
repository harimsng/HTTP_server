#include "Logger.hpp"
#include <cstdio>

#define	MAX_BUFFER_LEN (128)

using namespace std;

Logger::e_types		Logger::s_type = NONE;
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
	temp = NONE
		+ INFO * (type == "INFO")
		+ WARNING * (type == "WARNING")
		+ ERROR * (type == "ERROR")
		+ DEBUG * (type == "DEBUG");
	s_type = static_cast<e_types>(temp);
	cout << "[WARNING] vsnprintf() in Logger::log(e_Types, const char*, ...) is C++11(C99) feature. should be removed later.\n";
}

void
Logger::log(e_types type, const char* format, ...)
{
	std::string	prefix;
	char		buffer[MAX_BUFFER_LEN] = {0, };
	va_list		ap;

	if (s_type == NONE || s_type < type)
		return;
	va_start(ap, format);
	vsnprintf(buffer, MAX_BUFFER_LEN, format, ap);
	switch (type)
	{
		case INFO:
			prefix = "[INFO] ";
			break;
		case WARNING:
			prefix = "[WARNING] ";
			break;
		case ERROR:
			prefix = "[ERROR] ";
			break;
		case DEBUG:
			prefix = "[DEBUG] ";
			break;
		default:
			break;
	}
	prefix.append(Response::getDate("%F %T "));
	*s_ostream << prefix << buffer << '\n';
}
