#include "PutResponder.hpp"

// constructors & destructor
PutResponder::PutResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}

PutResponder::~PutResponder()
{
}

// operators
PutResponder&
PutResponder::operator=(const PutResponder& putResponder)
{
	(void)putResponder;
	return *this;
}

void
PutResponder::respond()
{
	switch (m_methodStatus)
	{
		case HEADER:
			respondHeader();
			openFile();
			m_methodStatus = BODY; // fall through
		case BODY:
			// if (isCgi)
			// cgiReadRequestBody();
			// else
			readRequestBody();
			if (m_methodStatus == BODY)
				break; // fall through
		case DONE:
			endResponse();
			break;
		default:
			;
	}
}
