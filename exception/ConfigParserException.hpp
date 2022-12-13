#ifndef CONFIGPARSEREXCEPTION_HPP
#define CONFIGPARSEREXCEPTION_HPP

#include <stdexcept>
#include <string>

#include "parser/config_parser/ConfigParser.hpp"

class	ConfigParser::ConfigParserException: public std::exception
{
public:
	ConfigParserException(const std::string& message): m_message("parse error: " + message) {};

	virtual ~ConfigParserException() throw() {};

	virtual const char*	what() const throw()
	{
		return m_message.c_str();
	}

private:
	std::string	m_message;
};

#endif
