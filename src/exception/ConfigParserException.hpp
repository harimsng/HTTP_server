#ifndef CONFIGPARSEREXCEPTION_HPP
#define CONFIGPARSEREXCEPTION_HPP

#include <stdexcept>
#include <string>

#include "parser/ConfigParser.hpp"

class	ConfigParser::ConfigParserException: public std::exception
{
public:
	ConfigParserException(const std::string& message);
	virtual ~ConfigParserException() throw();
	ConfigParserException(ConfigParserException const& configParserException);
	ConfigParserException&	operator=(ConfigParserException const& configParserException);

	virtual const char*	what() const throw();

private:
	std::string	m_message;
};

#endif
