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
PutResponder::respond() try
{
	if (m_request.m_status >= 300)
		throw (m_request.m_status);
	switch (m_responseStatus)
	{
		case RES_HEADER:
			openFile();
			m_responseStatus = RES_CONTENT; // fall through
		case RES_CONTENT:
			if (!(this->*m_recvContentFunc)())
				break;
			m_responseStatus = RES_CONTENT_FINISHED;// -> client_max_body_size error
		// fall through
		case RES_CONTENT_FINISHED:
			throw (201); // fall through
		case RES_DONE:
			endResponse();
			break;
		default:
			;
	}
}
catch (int ErrorstatusCode)
{
	string readBody;

	m_request.m_status = ErrorstatusCode;
	respondStatusLine(ErrorstatusCode);
	respondHeader();
	m_request.m_file.clear();
	m_request.m_path = getErrorPage(readBody);
	readFile(readBody);
	respondBody(readBody);
	endResponse();
}
