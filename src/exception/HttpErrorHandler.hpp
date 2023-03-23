#ifndef HTTPERRORHANDLER_HPP
#define HTTPERRORHANDLER_HPP

#include <string>

class	HttpErrorHandler
{
public:
	HttpErrorHandler(int errorCode);
	~HttpErrorHandler();

	const int	m_errorCode;
private:
	std::string	m_errorMessage;

	static std::string			s_errorMessageTable[1024];
	static void					setErrorMessageTable();
public:
	static const std::string&	getErrorMessage(int statusCode);
	const std::string&			getErrorMessage();
};

#endif
