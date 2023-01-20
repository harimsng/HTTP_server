#include <stdexcept>

#include "HttpErrorHandler.hpp"

extern const char*	g_httpVersion;

std::string	HttpErrorHandler::s_errorMessageTable[1024] = {"", };

void	HttpErrorHandler::setErrorMessageTable()
{
	if (s_errorMessageTable[200] != "OK")
	{
		s_errorMessageTable[200] = "OK";
		s_errorMessageTable[201] = "Created";
		s_errorMessageTable[202] = "Accepted";
		s_errorMessageTable[203] = "Non-Authoritative Information";
		s_errorMessageTable[204] = "No Content";
		s_errorMessageTable[205] = "Reset Content";
		s_errorMessageTable[206] = "Partial Content";
		s_errorMessageTable[300] = "Multiple Choices";
		s_errorMessageTable[301] = "Moved Permanently";
		s_errorMessageTable[302] = "Found";
		s_errorMessageTable[303] = "See Other";
		s_errorMessageTable[304] = "Not Modified";
		s_errorMessageTable[305] = "Use Proxy";
		s_errorMessageTable[307] = "Temporary Redirect";
		s_errorMessageTable[308] = "Permanent Redirect";
		s_errorMessageTable[400] = "Bad Request";
		s_errorMessageTable[401] = "Unauthorized";
		s_errorMessageTable[402] = "Payment Required";
		s_errorMessageTable[403] = "Forbidden";
		s_errorMessageTable[404] = "Not Found";
		s_errorMessageTable[405] = "Method Not Allowed";
		s_errorMessageTable[406] = "Not Acceptable";
		s_errorMessageTable[407] = "Proxy Authentication Required";
		s_errorMessageTable[408] = "Request Timeout";
		s_errorMessageTable[409] = "Conflict";
		s_errorMessageTable[410] = "Gone";
		s_errorMessageTable[411] = "Length Required";
		s_errorMessageTable[412] = "Precondition Failed";
		s_errorMessageTable[413] = "Content Too Large";
		s_errorMessageTable[414] = "URI Too Long";
		s_errorMessageTable[415] = "Unsupported Media Type";
		s_errorMessageTable[416] = "Range Not Satisfiable";
		s_errorMessageTable[417] = "Expectation Failed";
		s_errorMessageTable[421] = "Misdirected Request";
		s_errorMessageTable[422] = "Unprocessable Content";
		s_errorMessageTable[426] = "Upgrade Required";
		s_errorMessageTable[500] = "Internal Server Error";
		s_errorMessageTable[501] = "Not Implemented";
		s_errorMessageTable[502] = "Bad Gateway";
		s_errorMessageTable[503] = "Service Unavailable";
		s_errorMessageTable[504] = "Gateway Timeout";
		s_errorMessageTable[505] = "HTTP Version Not Supported";
	}
}

const std::string&	HttpErrorHandler::getErrorMessage(int statusCode)
{
	setErrorMessageTable();

	std::string&	errorMessage = s_errorMessageTable[statusCode];

	if (errorMessage.size() == 0)
		throw std::runtime_error("undefined http error code in HttpErrorHandler::getErrorMessage(int)");
	return errorMessage;
}

const std::string&
HttpErrorHandler::getErrorMessage()
{
	return (m_errorMessage);
}

// constructors & destructor
HttpErrorHandler::HttpErrorHandler(int errorCode)
{
	setErrorMessageTable();
	m_errorMessage = s_errorMessageTable[errorCode];
	if (m_errorMessage.size() == 0)
		throw std::runtime_error("undefined http error code in HttpErrorHandler::HttpErrorHandler(int)");
}
HttpErrorHandler::~HttpErrorHandler()
{
}

HttpErrorHandler::HttpErrorHandler(const HttpErrorHandler& httpErrorHandler)
{
	*this = httpErrorHandler;
}

// operators
HttpErrorHandler&
HttpErrorHandler::operator=(const HttpErrorHandler& httpErrorHandler)
{
	m_errorMessage = httpErrorHandler.m_errorMessage;
	return *this;
}
