#include <sys/stat.h>
#include <stdexcept>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "http/AMethod.hpp"
#include "http/GetMethod.hpp"
#include "http/HeadMethod.hpp"
#include "http/PutMethod.hpp"
#include "http/PostMethod.hpp"
#include "http/DeleteMethod.hpp"
#include "http/FindLocation.hpp"
#include "parser/HttpRequestParser.hpp"
#include "http/RequestHandler.hpp"

#define CHECK_PERMISSION(mode, mask) (((mode) & (mask)) == (mask))

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
	m_recvBuffer.setFd(m_socket->m_fd);
	m_sendBuffer.setFd(m_socket->m_fd);
}

RequestHandler::~RequestHandler()
{
}

int
RequestHandler::receiveRequest() try
{
	int			count;
	int			receiveStatus = RECV_NORMAL;

	if (m_sendBuffer.size() != 0)
		return RECV_SKIPPED;

	count = m_recvBuffer.receive(m_socket->m_fd);
	if (count == 0)
		return RECV_END;
	else if (count == -1)
		throw HttpErrorHandler(500);

	if (m_parser.m_readStatus < HttpRequestParser::HEADER_FIELDS_END)
		m_parser.parse(m_request);

	if (m_parser.m_readStatus == HttpRequestParser::HEADER_FIELDS_END)
	{
		//m_method->createResponseHeader();
		createResponseHeader();
		receiveStatus = RECV_EVENT;
	}
	if (m_parser.m_readStatus == HttpRequestParser::CONTENT)
		//m_method->createResponseContent
		m_method->completeResponse();

	return receiveStatus;
}
catch (HttpErrorHandler& e)
{
	bufferResponseStatusLine(400);
	bufferResponseHeaderFields();
	// generateResponse(404);
	return (0);
}

void
RequestHandler::sendResponse()
{
	m_sendBuffer.send(m_socket->m_fd);
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

// TODO: should be called on method classes
int
RequestHandler::resolveResourceLocation(const std::string& host)
{
	string			resourceLocation;
	Tcp::SocketAddr	addr = m_socket->getAddress();
	uint64_t		addrKey = Util::convertAddrKey(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
	VirtualServer*	server;
	if (ServerManager::s_virtualServerTable.count(addrKey) == 0)
		addrKey &= 0xffff00000000;
	if (ServerManager::s_virtualServerTable[addrKey].count(host) == 0)
		server = ServerManager::s_virtualServerTable[addrKey]["."];
	else
		server = ServerManager::s_virtualServerTable[addrKey][host];
	map<string, Location>&	locationTable = server->m_locationTable;

	FindLocation findLocation;

	resourceLocation = findLocation.saveRealPath(m_request, locationTable, server);

	return checkResourceStatus(resourceLocation.data());
}

int
RequestHandler::checkResourceStatus(const char* path)
{
	int			ret;
	struct stat	status;
	int			statusCode;

	ret = stat(path, &status);
	if (ret == 0
		&& S_ISREG(status.st_mode)
		&& CHECK_PERMISSION(status.st_mode,
//					S_IWUSR | S_IWGRP | S_IWOTH // for DELETE
					S_IRUSR | S_IRGRP | S_IROTH // for GET, HEAD
//					S_IXUSR | S_IXGRP | S_IXOTH // for POST, PUT
					))
		statusCode = 200;

	switch (errno)
	{
		case EACCES:
			// fall through
		case ENOENT:
			// fall through
		case ENOTDIR:
			statusCode = 404;
			break;
		case ENAMETOOLONG:
			statusCode = 414;
			break;
		default:
			statusCode = 500;
			break;
	}
	return statusCode;
}

// HOST field could be empty
void
RequestHandler::createResponseHeader()
{
	string		resourceLocation;
	int			statusCode;

	checkRequestMessage();
// TODO: which should be first between method creation and uri checking
	statusCode = resolveResourceLocation(m_request.m_headerFieldsMap["HOST"][0]);
	bufferResponseStatusLine(statusCode);
	bufferResponseHeaderFields();
	switch (m_request.m_method)
	{
		case GET:
			m_method = new GetMethod(m_request, m_sendBuffer, m_recvBuffer);
			break;
		case HEAD:
			m_method = new HeadMethod(m_request, m_sendBuffer, m_recvBuffer);
			break;
		case POST:
			m_method = new PostMethod(m_request, m_sendBuffer, m_recvBuffer);
			break;
		case PUT:
			m_method = new PutMethod(m_request, m_sendBuffer, m_recvBuffer);
			break;
		case DELETE:
			m_method = new DeleteMethod(m_request, m_sendBuffer, m_recvBuffer);
			break;
		default: ;
			// throw HttpErrorHandler(???);
	}
	m_parser.m_readStatus = HttpRequestParser::CONTENT;
}

void
RequestHandler::bufferResponseStatusLine(int statusCode)
{
	// status-line
	m_sendBuffer.append(g_httpVersion);
	m_sendBuffer.append(" ");
	m_sendBuffer.append(Util::toString(statusCode));
	m_sendBuffer.append(" ");
	// m_sendBuffer.append(status code message);
	m_sendBuffer.append(g_CRLF);
}

void
RequestHandler::bufferResponseHeaderFields()
{
	// m_recvBuffer.receive(m_socket->m_fd);

	// m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append("Date: " + Util::getDate("%a, %d %b %Y %X %Z"));
	m_sendBuffer.append(g_CRLF);
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
