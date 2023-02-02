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
	switch (m_responseStatus)
	{
		case RES_HEADER:
			respondHeader();
			openFile(); // fall through
		case RES_CONTENT:
			// if (isCgi)
			// cgiReadRequestBody();
			// else
			readRequestBody();
			if (m_responseStatus == RES_CONTENT)
				break; // fall through
		case RES_DONE:
			endResponse();
			break;
		default:
			;
	}
}
