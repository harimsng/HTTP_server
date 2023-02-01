#include <fcntl.h>
#include <iostream>

#include "http/AutoIndex.hpp"
#include "util/Util.hpp"
#include "responder/GetResponder.hpp"

using namespace std;

// constructors & destructor
GetResponder::GetResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}

GetResponder::~GetResponder()
{
}

// operators
GetResponder&
GetResponder::operator=(const GetResponder& getMethod)
{
	(void)getMethod;
	return *this;
}

void
GetResponder::respond()
{
	string readBody;

	switch (m_methodStatus)
	{
		case HEADER:
			respondHeader();
			m_methodStatus = BODY; // fall through
		case BODY:
			if (m_request.m_file == "")
				readBody = AutoIndex::autoIndex(m_request.m_path, m_request.m_uri);
			else
				readFile(readBody);
			m_sendBuffer.append("Content-Length: ");
			m_sendBuffer.append(Util::toString(readBody.size()));
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.reserve(m_sendBuffer.size() + readBody.size());

			// TODO: change code to use swap instead of appen
			m_sendBuffer.append(readBody);
			m_methodStatus = DONE; // fall through
			// break;
		case DONE:
		// method must know end of response(content length, chunked)
			endResponse();
			break;
		default:
			;
	}
}
