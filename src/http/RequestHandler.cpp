#include <stdexcept>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "http/AMethod.hpp"
#include "http/RequestHandler.hpp"
#include "parser/HttpRequestParser.hpp"

#define REQUEST_EOF (0)

using namespace	std;

// forbidden
RequestHandler::RequestHandler(const RequestHandler& requestHandler)
:	m_socket(NULL),
	m_parser(m_recvBuffer),
	m_method(NULL)
{
	(void)requestHandler;
}

RequestHandler&
RequestHandler::operator=(const RequestHandler& request)
{
	(void)request;
	return *this;
}

// constructors & destructor
RequestHandler::RequestHandler(const Socket<Tcp>& socket)
:	m_socket(&socket),
	m_parser(m_recvBuffer),
	m_method(NULL)
{
}

RequestHandler::~RequestHandler()
{
}

int
RequestHandler::receiveRequest() try
{
	int			count;

	count = m_recvBuffer.receive(m_socket->m_fd);
	cout << "count : " << count << endl;
	if (count == 0)
		return REQUEST_EOF;
	else if (count == -1)
		throw HttpErrorHandler(500);

	if (m_parser.m_readStatus < HttpRequestParser::HEADER_FIELDS_END)
		m_parser.parse(m_request);
	if (m_parser.m_readStatus == HttpRequestParser::HEADER_FIELDS_END)
		makeResponseHeader();
	if (m_parser.m_readStatus == HttpRequestParser::BODY_FIELDS)
		m_method->completeResponse();
	return count;
}
catch(HttpErrorHandler& e)
{
	// generateResponse(404);
	return (0);
}

// HOST field could be empty
void
RequestHandler::makeResponseHeader()
{
	string		rl;

	checkRequestMessage();
	rl = getResourceLocation(m_request.m_headerFieldsMap["host"][0]);
	Util::checkFileStat(rl.data());
	// status code 200
	// make response header
	switch (m_request.m_method)
	{
		default:
			m_method = new AMethod(m_request, m_sendBuffer, m_recvBuffer);
	}
	generateResponse(200);
	m_parser.m_readStatus = HttpRequestParser::BODY_FIELDS;
}

void
RequestHandler::checkRequestMessage()
{
	// 1. check method
	// 2. check uri length
	// 3. check http version
	// 4. check reqeust header
	// 4.1 ckeck host field
	// 5. check reqeust body size

	checkStatusLine(); // 1, 2, 3
	checkHeaderFields(); // 4
}

void
RequestHandler::checkStatusLine()
{
	if (m_request.m_method == ERROR) // check method
	{}
	// if (m_request.m_uri >= uri_size) // check uri length
	if (m_request.m_protocol != "HTTP/1.1") // check http version
	{}
}

void
RequestHandler::checkHeaderFields()
{
	bool check = true;

	check &= m_request.m_headerFieldsMap.count("HOST") > 0;

	// ... check allow_method field

	if (check == false)
		throw HttpErrorHandler(400);
}

std::string
RequestHandler::getResourceLocation(const std::string& host)
{
	Tcp::SocketAddr	addr = m_socket->getAddress();
	uint64_t		addrKey = (static_cast<uint64_t>(ntohs(addr.sin_port)) << 32) + ntohl(addr.sin_addr.s_addr);
	VirtualServer*	server = ServerManager::s_virtualServerTable[addrKey][host];
	map<string, Location>&	locationTable = server->m_locationTable;

	(void)locationTable;
	return "";
}

void
RequestHandler::generateResponse(int statusCode)
{
	// status-line
	m_sendBuffer.append(g_httpVersion);
	m_sendBuffer.append(" ");
	m_sendBuffer.append(Util::toString(statusCode));
	m_sendBuffer.append(" ");
	m_sendBuffer.append(g_CRLF);
	//
	//
	m_sendBuffer.append(g_CRLF);
}

void
RequestHandler::sendResponse()
{
	m_recvBuffer.receive(m_socket->m_fd);
}

void
RequestHandler::makeErrorResponse(const std::string& errorMessage)
{
	(void)errorMessage;
}

std::ostream&
operator<<(std::ostream& os, const Request& request)
{
	HeaderFieldsMap::const_iterator mapIt;

	os << "reqeust info\n";
	os << "status line\n";
	os << "\tmethod : " << request.m_method << endl;
	os << "\ttarget : " << request.m_uri << endl;
	os << "\tprotocol : " << request.m_protocol << endl;
	os << "header field";
	for (mapIt = request.m_headerFieldsMap.begin();
			mapIt != request.m_headerFieldsMap.end(); mapIt++)
	{
		os << "\n\t" << mapIt->first << " : ";
		vector<string>::const_iterator vecIt = mapIt->second.begin();
		for (; vecIt != mapIt->second.end(); vecIt++)
			os << *vecIt << " ";
	}
	return (os);
}
