#include "Response.hpp"

// constructors & destructor
Response::Response(const Socket<Tcp>& socket)
:	m_socket(&socket)
{
}

Response::Response(const Socket<Tcp>& socket, HttpInfo& httpInfo)
:	m_socket(&socket),
	m_httpInfo(&httpInfo)
{
}

Response::~Response()
{
}

Response::Response(const Response& response)
{
	(void)response;
}

// operators
Response&
Response::operator=(const Response& response)
{
	(void)response;
	return *this;
}

void
Response::sendResponse(int eventInfo)
{
	(void)eventInfo;
}

void
Response::makeErrorResponse(const std::string& errorMessage)
{
	(void)m_socket;
	(void)m_httpInfo;
	(void)errorMessage;
}
