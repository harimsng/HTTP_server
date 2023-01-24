#include "DeleteMethod.hpp"

// constructors & destructor
DeleteMethod::DeleteMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer)
: AMethod(request, sendBuffer, recvBuffer)
{
}

DeleteMethod::~DeleteMethod()
{
}

// operators
DeleteMethod&
DeleteMethod::operator=(const DeleteMethod& deleteMethod)
{
	(void)deleteMethod;
	return *this;
}

void
DeleteMethod::completeResponse()
{
}

