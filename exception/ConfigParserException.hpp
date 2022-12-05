#ifndef CONFIGPARSEREXCEPTION_HPP
#define CONFIGPARSEREXCEPTION_HPP

#include <exception>
#include <string>

using namespace std;

class	ConfigParserException : public exception
{
public:
// constructors & destructor
	ConfigParserException(const string& errMsg);
	~ConfigParserException() throw();
	ConfigParserException(ConfigParserException const& configParserException);

// operators
	ConfigParserException	&operator=(ConfigParserException const& configParserException);

// member functions
	const char *what() const throw();

private:
	string m_errMsg;
};

#endif
