#ifndef LOGGER_HPP
#define LOGGER_HPP

# include <string>
# include <iostream>
# include <iomanip>

# include "util/Util.hpp"

# if 0
#  define LOG(type, fmt, ...)\
		Logger::log(Logger::type, fmt, ##__VA_ARGS__);
# else
// VERBOSE DEBUG
#  define LOG(type, fmt, ...)\
	if (Logger::type == Logger::DEBUG)\
	{\
		Logger::log(Logger::DEBUG, "%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);\
	}\
	else\
	{\
		Logger::log(Logger::type, fmt, ##__VA_ARGS__);\
	}
# endif

class	Logger
{
// hidden
	Logger();
	Logger(Logger const& logger);
	Logger	&operator=(Logger const& logger);

public:
	enum	e_types
	{
		DISABLED = 0,
		INFO = 1,
		WARNING = 2,
		ERROR = 3,
		DEBUG = 4,
		VERBOSE = 5,
	};

// constructors & destructor
	~Logger();

// member functions
	template <typename T>
	static void	log(e_types type, const T& object);
	static void	log(e_types type, const char* format, ...);
	static void	initLogger(const std::string& type = "INFO", std::ostream& os = std::cerr);

private:
	static e_types			s_type;
	static std::ostream*	s_ostream;
};

/*
 * note: sync code with void Logger::log(e_types type, const char* format, ...)
 */
template <typename T>
void
Logger::log(e_types type, const T& object)
{
	std::string	prefix;
	std::string	suffix;

	if (s_type == DISABLED || s_type < type)
		return;
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
			suffix = " (" + Util::toString(errno) + " "
				+ std::strerror(errno) + ")";
			break;
		case DEBUG:
			prefix = "[DEBUG] ";
			break;
		default:
			break;
	}
	prefix.append(Util::getDate("%F %T "));
	*s_ostream << prefix << object << suffix << '\n';
}

#endif
