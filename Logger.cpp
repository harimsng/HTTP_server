#include "Logger.hpp"

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
}
