#include <unistd.h>
#include <stdexcept>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "http/RequestHandler.hpp"
#include "parser/HttpRequestParser.hpp"

#define REQUEST_BUFFER_SIZE (8192)
#define REQUEST_BUFFER_RESERVE (REQUEST_BUFFER_SIZE + (REQUEST_BUFFER_SIZE >> 8))
#define REQUEST_EOF (0)

using namespace	std;

// forbidden
RequestHandler::RequestHandler(const RequestHandler& requestHandler)
:	m_socket(NULL),
	m_parser(m_recvBuffer)
{
	m_recvBuffer.reserve(REQUEST_BUFFER_RESERVE);
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
	m_parser(m_recvBuffer)
{
	m_recvBuffer.reserve(REQUEST_BUFFER_RESERVE);
}

RequestHandler::~RequestHandler()
{
}

int
RequestHandler::receiveRequest()
{
	int			count;

	count = receiveRawData();
	if (count == 0)
		return REQUEST_EOF;
	else if (count == -1)
		throw HttpErrorHandler(500);
	m_parser.parse(m_request);
	return count;
}

int
RequestHandler::receiveRawData()
{
	const int	residue = m_recvBuffer.size();
	int			count = 0;

#ifdef __APPLE__
	m_buffer.resize(REQUEST_BUFFER_SIZE, 0);
	count = ::read(m_socket->m_fd, const_cast<char*>(m_buffer.data()) + residue,
			m_buffer.size() - residue - 1);
	m_buffer.resize(residue + count, 0);

#elif __linux__
	m_recvBuffer.resize(REQUEST_BUFFER_SIZE, 0);
	count = ::read(m_socket->m_fd, const_cast<char*>(m_recvBuffer.data()) + residue,
			m_recvBuffer.size() - residue - 1);
	m_recvBuffer.resize(residue + count + 1, 0);
#endif
	return count;
}

void
RequestHandler::makeResponse()
{
	switch (m_parser.m_readStatus)
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
RequestHandler::makeResponseHeader()
{
	string	rl = getResourceLocation(m_request.m_headerFieldsMap["host"][0]);

	if (m_request.m_method == RequestHandler::GET)
	{
		makeResponseBody();
		m_parser.m_readStatus = HttpRequestParser::FINISHED;
		return;
	}
	m_parser.m_readStatus = HttpRequestParser::MESSAGE_BODY;
}

void
RequestHandler::makeResponseBody()
{
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
RequestHandler::sendResponse()
{
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
