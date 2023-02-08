#include "HeadResponder.hpp"

// constructors & destructor
HeadResponder::HeadResponder(RequestHandler& requesthandler)
:	AResponder(requesthandler)
{
}

HeadResponder::~HeadResponder()
{
}

// operators
HeadResponder&
HeadResponder::operator=(const HeadResponder& HeadResponder)
{
	(void)HeadResponder;
	return *this;
}

void
HeadResponder::respondWork()
{
	switch(m_responseStatus)
	{
		case RES_HEADER:
			respondStatusLine(m_request.m_status);
			respondHeader();
			m_sendBuffer.append(g_CRLF);
			m_responseStatus = RES_DONE; // fall through
		case RES_DONE:
			endResponse();
			break;
		default:
			;
	}
}
