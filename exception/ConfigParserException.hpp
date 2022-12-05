#ifndef CONFIGPARSEREXCEPTION_HPP
#define CONFIGPARSEREXCEPTION_HPP

class	ConfigParserException
{
public:
// constructors & destructor
	ConfigParserException();
	~ConfigParserException();
	ConfigParserException(ConfigParserException const& configParserException);

// operators
	ConfigParserException	&operator=(ConfigParserException const& configParserException);

// member functions
};

#endif
