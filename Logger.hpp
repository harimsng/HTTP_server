#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>

const std::string	getDate(const char* format);

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
		ERROR = 1,
		WARNING = 2,
		INFO = 3,
		DEBUG = 4
	};

// constructors & destructor
	~Logger();

// member functions
	template <typename T>
	static void	log(e_types type, const T& object);
	static void	log(e_types type, const char* format, ...);
	static void	initLogger(const std::string& type, std::ostream& os = std::cerr);

	static e_types			s_type;
	static std::ostream*	s_ostream;
};

template <typename T>
void
Logger::log(e_types type, const T& object)
{
	std::string	prefix;

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
			break;
		case DEBUG:
			prefix = "[DEBUG] ";
			break;
		default:
			break;
	}
	prefix.append(::getDate("%F %T "));
	*s_ostream << prefix << object << '\n';
}

#endif
