#ifndef LOGGER_HPP
#define LOGGER_HPP

# include <string>
# include <iomanip>
# include <cerrno>
# include <cstring>
# include <iostream>

# include "util/Util.hpp"

# if 0
#  define LOG(type, fmt, ...)\
		Logger::log(Logger::type, fmt, ##__VA_ARGS__);
# else
#  define LOG(type, fmt, ...)\
	if (Logger::type >= Logger::DEBUG)\
	{\
		Logger::log(Logger::type, "%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__);\
	}\
	else\
	{\
		Logger::log(Logger::type, fmt, ##__VA_ARGS__);\
	}
# endif

class	Logger
{
	Logger();
	Logger(Logger const& logger);
	Logger	&operator=(Logger const& logger);

public:
	enum	e_types
	{
		DISABLED = 0,
		ERROR = 1,
		WARNING = 2,
		INFO = 3,
		DEBUG = 4,
		VERBOSE = 5
	};

	~Logger();

	template <typename T>
	static void	log(e_types type, const T& object);
	static void	log(e_types type, const char* format, ...);
	static void	initLogger(const std::string& type = "INFO", std::ostream& os = std::cerr);

private:
	static e_types			s_type;
	static std::ostream*	s_ostream;
	static const char*		s_prefixTable[];
};

template <typename T>
void
Logger::log(e_types type, const T& object)
{
	std::string	prefix;
	std::string	suffix;

	if (s_type == DISABLED || s_type < type)
		return;

	prefix = s_prefixTable[type];
	if (type == ERROR)
	{
		suffix = " (" + Util::toString(errno) + " "
			+ std::strerror(errno) + ")";
	}
	if (type == VERBOSE)
	{
		prefix.append(Util::getDate("%F %T "));
	}
	*s_ostream << prefix << object << suffix << std::endl;
}

#endif
