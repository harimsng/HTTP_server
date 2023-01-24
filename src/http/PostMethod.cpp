#include "PostMethod.hpp"
#include "http/AMethod.hpp"

// constructors & destructor
PostMethod::PostMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer) : AMethod(request, sendBuffer, recvBuffer)
{
}
PostMethod::~PostMethod()
{
}

// operators
PostMethod&
PostMethod::operator=(const PostMethod& postMethod)
{
	(void)postMethod;
	return *this;
}

void
PostMethod::completeResponse()
{
}

