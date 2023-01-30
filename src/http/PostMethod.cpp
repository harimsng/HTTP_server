#include "PostMethod.hpp"

// constructors & destructor
PostMethod::PostMethod(RequestHandler& requestHandler)
:	AMethod(requestHandler)
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

