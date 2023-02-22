#include "PutResponder.hpp"

using namespace std;

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
PutResponder::respondWork()
{
	switch (m_responseStatus)
	{
		case RES_HEADER:
			openFile();
			m_responseStatus = RES_CONTENT; // fall through
		case RES_CONTENT:
			if (!(this->*m_recvContentFunc)())
				break;
			close(m_fileFd);
			m_responseStatus = RES_CONTENT_FINISHED;
		// fall through
		case RES_CONTENT_FINISHED:
			throw (201); // fall through
		case RES_DONE:
			close(m_fileFd);
			endResponse();
			break;
		default:
			;
	}
}
