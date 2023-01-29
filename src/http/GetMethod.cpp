#include <fcntl.h>
#include <iostream>

#include "ServerManager.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"
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

	readFile(readBody);
	m_sendBuffer.append("Content-Length: ");
	m_sendBuffer.append(Util::toString(readBody.size()));
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(readBody);

	// method must know end of response(content length, chunked)
	endResponse();
}
