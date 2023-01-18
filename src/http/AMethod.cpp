#include "http/RequestHandler.hpp"
#include "AMethod.hpp"

// constructors & destructor
AMethod::AMethod()
{
}

AMethod::~AMethod()
{
}

AMethod::AMethod(const AMethod& aMethod)
{
	(void)aMethod;
}

AMethod::AMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer)
{
	(void)request;
	(void)sendBuffer;
	(void)recvBuffer;
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
