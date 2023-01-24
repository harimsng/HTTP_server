#include "GetMethod.hpp"
#include "http/AMethod.hpp"

// constructors & destructor

GetMethod::GetMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer): AMethod(request, sendBuffer, recvBuffer)
{
}

GetMethod::~GetMethod()
{
}

// operators
GetMethod&
GetMethod::operator=(const GetMethod& getMethod)
{
	(void)getMethod;
	return *this;
}

void
GetMethod::completeResponse()
{

}
