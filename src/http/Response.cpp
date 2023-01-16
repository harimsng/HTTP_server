#include "ServerManager.hpp"
#include "parser/HttpRequestParser.hpp"
#include "Response.hpp"

// for test
#include <iostream>

using namespace std;

// constructors & destructor
Response::Response(Request& request)
:	m_request(&request)
{
}

Response::~Response()
{
}

Response&
Response::operator=(const Response& response)
{
	m_request = response.m_request;
	m_buffer = response.m_buffer;
	return *this;
}

// operator 
Response::Response(const Response& response)
{
	*this = response;
}

// member functions
void
Response::makeResponse()
{
	switch (m_request->m_parser.m_readStatus)
	{
		case HttpRequestParser::REQUEST_LINE:
		case HttpRequestParser::HEADER_FIELDS:
			return;
		case HttpRequestParser::HEADER_FIELDS_END:
			makeResponseHeader();
			break;
		case HttpRequestParser::MESSAGE_BODY:
			makeResponseBody();
			break;
		case HttpRequestParser::FINISHED:
			;
	};
}

// HOST field could be empty
void
Response::makeResponseHeader()
{
	string	rl = getResourceLocation(m_request->m_headerFieldsMap["host"][0]);

	if (m_request->m_method == Request::GET)
	{
		makeResponseBody();
		m_request->m_parser.m_readStatus = HttpRequestParser::FINISHED;
		return;
	}
	m_request->m_parser.m_readStatus = HttpRequestParser::MESSAGE_BODY;
}

void
Response::makeResponseBody()
{
}

std::string
Response::getResourceLocation(const std::string& host)
{
	Tcp::SocketAddr	addr = m_request->m_socket->getAddress();
	uint64_t		addrKey = (static_cast<uint64_t>(ntohs(addr.sin_port)) << 32) + ntohl(addr.sin_addr.s_addr);
	VirtualServer*	server = ServerManager::s_virtualServerTable[addrKey][host];
	map<string, Location>&	locationTable = server->m_locationTable;

	(void)locationTable;
	return "";
}

void
Response::sendResponse()
{
}

void
Response::makeErrorResponse(const std::string& errorMessage)
{
	(void)m_httpInfo;
	(void)errorMessage;
}
