#ifndef HTTPERRORHANDLER_HPP
#define HTTPERRORHANDLER_HPP

#include <string>

class	HttpErrorHandler
{
public:
// constructors & destructor
	HttpErrorHandler(int errorCode);
	~HttpErrorHandler();
	HttpErrorHandler(const HttpErrorHandler& httpErrorHandler);
	HttpErrorHandler	&operator=(const HttpErrorHandler& httpErrorHandler);

// member functions
private:
	std::string	m_errorMessage;

	static std::string			s_errorMessageTable[1024];
	static void					setErrorMessageTable();
public:
	static const std::string&	getErrorMessage(int errorCode);
	const std::string&			getErrorMessage();
};

#endif
