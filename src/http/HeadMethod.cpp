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
	switch(m_methodStatus)
	{
		case HEADER:
			completeResponseHeader();
			m_sendBuffer.append(g_CRLF);
			m_methodStatus = DONE;
		case DONE:
			endResponse();
			break;
		default:
			;
	}
}
