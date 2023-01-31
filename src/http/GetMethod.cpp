#include <fcntl.h>
#include <iostream>

#include "ServerManager.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp" // is it necessary?
#include "AutoIndex.hpp"
#include "GetMethod.hpp"

using namespace std;

// constructors & destructor
GetMethod::GetMethod(RequestHandler& requestHandler)
:	AMethod(requestHandler)
{
}

GetMethod::~GetMethod()
{
}

// operators
GetMethod&
GetMethod::operator=(const GetMethod& getMethod)
{
	(void)getMethod;
	return *this;
}

void
GetMethod::completeResponse()
{
	string readBody;

	switch (m_methodStatus)
	{
		case HEADER:
			completeResponseHeader();
			m_methodStatus = BODY;

		case BODY:
			readBody = "";
			if (m_request.m_file == "")
				readBody = AutoIndex::autoIndex(m_request.m_path, m_request.m_uri);
			else
				readFile(readBody);
			m_sendBuffer.append("Content-Length: ");
			m_sendBuffer.append(Util::toString(readBody.size()));
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.append(readBody);
			m_methodStatus = DONE;
			// break;

		case DONE:
		// method must know end of response(content length, chunked)
			endResponse();
			break;
		default:
			;
	}
}
