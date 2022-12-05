#include "HttpRequestParserException.hpp"

// constructors & destructor
HttpRequestParserException::HttpRequestParserException()
{
}

HttpRequestParserException::~HttpRequestParserException()
{
}

HttpRequestParserException::HttpRequestParserException(HttpRequestParserException const& httpRequestParserException)
{
	(void)httpRequestParserException;
}

// operators
HttpRequestParserException&	HttpRequestParserException::operator=(HttpRequestParserException const& httpRequestParserException)
{
	(void)httpRequestParserException;
	return *this;
}
