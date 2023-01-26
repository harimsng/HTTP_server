#include "http/RequestHandler.hpp"
#include "io/Buffer.hpp"
#include "util/Util.hpp"
#include "AMethod.hpp"

// constructors & destructor
AMethod::AMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer)
	:m_request(request),
	m_sendBuffer(sendBuffer),
	m_recvBuffer(recvBuffer)
{
}

AMethod::~AMethod()
{
}


// operators
AMethod&
AMethod::operator=(const AMethod& aMethod)
{
	(void)aMethod;
	return *this;
}

// member functions
void
AMethod::completeResponse()
{
}
