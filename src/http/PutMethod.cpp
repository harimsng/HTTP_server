#include "http/AMethod.hpp"
#include "PutMethod.hpp"

// constructors & destructor
PutMethod::PutMethod(RequestHandler& requestHandler)
:	AMethod(requestHandler)
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

