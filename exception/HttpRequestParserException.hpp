#ifndef HTTPREQUESTPARSEREXCEPTION_HPP
#define HTTPREQUESTPARSEREXCEPTION_HPP

class	HttpRequestParserException
{
public:
// constructors & destructor
	HttpRequestParserException();
	~HttpRequestParserException();
	HttpRequestParserException(HttpRequestParserException const& httpRequestParserException);

// operators
	HttpRequestParserException	&operator=(HttpRequestParserException const& httpRequestParserException);

// member functions
};

#endif
