#include "Communicator.hpp"
#include "Request.hpp"
#include "Server.hpp"

#include <iostream>

using namespace std;

Communicator::Communicator()
{
	m_preBufferSize = 0;
}

Communicator::~Communicator()
{
}

// int
// Communicator::readRequestHeader(string& requestMessageBuffer) try
// {
//     if (m_request.m_requestSection == Request::REQUEST_LINE)
//         requestMessageBuffer.erase(0, m_request.makeRequestLine(requestMessageBuffer, m_methodType, m_uri));
//     if (m_request.m_requestSection == Request::REQUEST_HEADER)
//         requestMessageBuffer.erase(0, m_request.makeRequestHeader(requestMessageBuffer, m_requestHeader));
//     if (m_request.m_requestSection == Request::REQUEST_HEADER_END)
//         return (m_request.m_requestSection);
//     m_preBufferSize = requestMessageBuffer.size();
//     return (m_request.m_requestSection);
// }
// catch(exception& e)
// {
//     return (m_request.m_requestSection);
// }

void
Communicator::writeSocket(int fd)
{
	(void)fd;
	// wirte(fd, );
}

int	 Communicator::readRequestBody(std::string& requestMessageBuffer)
{
	m_request.makeReqeustBody(requestMessageBuffer);
	return (1);
}
