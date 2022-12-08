#include "ConfigParserException.hpp"

// constructors & destructor
ConfigParserException::ConfigParserException()
{
}

ConfigParserException::~ConfigParserException()
{
}

ConfigParserException::ConfigParserException(ConfigParserException const& configParserException)
{
	(void)configParserException;
}

// operators
ConfigParserException&	ConfigParserException::operator=(ConfigParserException const& configParserException)
{
	(void)configParserException;
	return *this;
}
