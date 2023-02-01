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
HeadResponder::respond()
{
	switch(m_methodStatus)
	{
		case HEADER:
			respondHeader();
			m_sendBuffer.append(g_CRLF);
			m_methodStatus = DONE; // fall through
		case DONE:
			endResponse();
			break;
		default:
			;
	}
}
