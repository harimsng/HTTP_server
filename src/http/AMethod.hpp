#ifndef AMETHOD_HPP
#define AMETHOD_HPP

#include <sys/stat.h>
#include <sstream>
#include <fstream>

#include "io/Buffer.hpp"
#include "util/Util.hpp"
#include "http/RequestHandler.hpp"

class	AMethod
{
private:
	AMethod();
	AMethod(const AMethod& method);

public:
// constructors & destructor
	AMethod(RequestHandler& requestHandler);
	~AMethod();

// operators
	AMethod	&operator=(const AMethod& aMethod);

// member functions
	virtual void	completeResponse() = 0;
	void			endResponse();

	void			readFile(std::string& readBody);
	bool			checkFileExists(const std::string& filePath);
	bool			checkDirExists(const std::string& filePath);

protected:
	RequestHandler&		m_requestHandler;
	Request&			m_request;
	SendBuffer&			m_sendBuffer;
	ReceiveBuffer&		m_recvBuffer;
};

#endif
