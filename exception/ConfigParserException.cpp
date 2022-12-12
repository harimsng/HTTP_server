#include "ConfigParserException.hpp"

// constructors & destructor
ConfigParserException::ConfigParserException(const string& errMsg)
{
	m_errMsg = errMsg;
}

ConfigParserException::~ConfigParserException() throw()
{
}

ConfigParserException::ConfigParserException(ConfigParserException const& configParserException)
{
	(void)configParserException;
}

// operators
ConfigParserException&
ConfigParserException::operator=(ConfigParserException const& configParserException)
{
	(void)configParserException;
	return *this;
}

const char*
ConfigParserException::what() const throw()
{
	return (m_errMsg.c_str());
}
