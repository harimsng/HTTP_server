#ifndef AMETHOD_HPP
#define AMETHOD_HPP

#include "io/Buffer.hpp"
#include "parser/HttpRequestParser.hpp"
#include "fstream"
#include "util/Util.hpp"

struct	Request;

class	AMethod
{
private:
	AMethod();
	AMethod(const AMethod& method);
public:
// constructors & destructor
	AMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer);
	~AMethod();

// operators
	AMethod	&operator=(const AMethod& aMethod);

// member functions
	virtual void	completeResponse() = 0;

	Request&		m_request;
	SendBuffer&		m_sendBuffer;
	ReceiveBuffer&	m_recvBuffer;
};

#endif
