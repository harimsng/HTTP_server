#include "exception/ConfigParserException.hpp"

using namespace	std;

// constructors & destructor
ConfigParser::ConfigParserException::ConfigParserException(const string& message)
{
	m_message = message;
}

ConfigParser::ConfigParserException::~ConfigParserException() throw()
{
}

ConfigParser::ConfigParserException::ConfigParserException(ConfigParserException const& configParserException)
{
	*this = configParserException;
}

// operators
ConfigParser::ConfigParserException&
ConfigParser::ConfigParserException::operator=(ConfigParserException const& configParserException)
{
	m_message = configParserException.m_message;
	return *this;
}

const char*
ConfigParser::ConfigParserException::what() const throw()
{
	return (m_message.c_str());
}
