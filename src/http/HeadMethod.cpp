#include "HeadMethod.hpp"
#include "http/AMethod.hpp"

// constructors & destructor

HeadMethod::HeadMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer) : AMethod(request, sendBuffer, recvBuffer)
{
}

HeadMethod::~HeadMethod()
{
}

// operators
HeadMethod&
HeadMethod::operator=(const HeadMethod& headMethod)
{
	(void)headMethod;
	return *this;
}

void
HeadMethod::completeResponse()
{
}

