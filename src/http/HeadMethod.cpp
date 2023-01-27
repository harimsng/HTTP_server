#include "http/AMethod.hpp"
#include "HeadMethod.hpp"

// constructors & destructor

HeadMethod::HeadMethod(RequestHandler& requestHandler)
:	AMethod(requestHandler)
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

