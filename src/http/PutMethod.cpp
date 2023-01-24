#include "PutMethod.hpp"
#include "http/AMethod.hpp"

// constructors & destructor
PutMethod::PutMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer) : AMethod(request, sendBuffer, recvBuffer)
{
}

PutMethod::~PutMethod()
{
}

// operators
PutMethod&
PutMethod::operator=(const PutMethod& putMethod)
{
	(void)putMethod;
	return *this;
}

void PutMethod::completeResponse()
{
}

