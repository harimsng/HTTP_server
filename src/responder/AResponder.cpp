#include "AResponder.hpp"

// deleted
AResponder::AResponder(const AResponder& aresponder)
:	m_requestHandler(NULL)
{
	(void)aresponder;
}

AResponder::~AResponder()
{
}

// operators
AResponder&
AResponder::operator=(const AResponder& aresponder)
{
	(void)aresponder;
	return *this;
}
// constructors & destructor
AResponder::AResponder(const RequestHandler& requestHandler)
:	m_requestHandler(&requestHandler)
{
}
