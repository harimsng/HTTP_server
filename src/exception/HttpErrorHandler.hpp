#ifndef HTTPERRORHANDLER_HPP
#define HTTPERRORHANDLER_HPP

#include <string>

class	HttpErrorHandler
{
public:
// constructors & destructor
	HttpErrorHandler(int errorCode);
	~HttpErrorHandler();
//	HttpErrorHandler(const HttpErrorHandler& httpErrorHandler) = default;
//	HttpErrorHandler	&operator=(const HttpErrorHandler& httpErrorHandler) = default;

// member functions
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
