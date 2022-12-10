#ifndef CONFIGPARSEREXCEPTION_HPP
#define CONFIGPARSEREXCEPTION_HPP

#include <stdexcept>
#include <string>

#include "ConfigParser.hpp"

class	ConfigParser::ConfigParserException: public std::exception
{
public:
	ConfigParserException(const std::string& message): m_message("ConfigParserException: " + message) {};

	virtual ~ConfigParserException() throw() {};

	virtual const char*	what() const throw()
	{
		return m_message.c_str();
	}

private:
	std::string	m_message;
};

#endif
