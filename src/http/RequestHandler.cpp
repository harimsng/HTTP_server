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
}

RequestHandler::~RequestHandler()
{
}

int
RequestHandler::receiveRequest() try
{
	int			count;

	if (m_sendBuffer.size() != 0)
		return RECV_SKIPPED;

	count = m_recvBuffer.receive(m_socket->m_fd);
	if (count == 0)
		return RECV_END;
	else if (count == -1)
		throw HttpErrorHandler(500);

	switch (m_parser.m_readStatus)
	{
		case HttpRequestParser::REQUEST_LINE_METHOD:
		case HttpRequestParser::REQUEST_LINE:
		case HttpRequestParser::HEADER_FIELDS:
			m_parser.parse(m_request);
		case HttpRequestParser::HEADER_FIELDS_END:
			createResponseHeader();
		case HttpRequestParser::CONTENT:
			m_method->completeResponse();
		case HttpRequestParser::FINISHED:
			m_recvBuffer.clear(); // ignores excessive data
	}
	return count;
}
catch(HttpErrorHandler& e)
{
	cout << "error" << endl;
	// generateResponse(404);
	return (0);
}

void
RequestHandler::sendResponse()
{
	string		rl;

	checkRequestMessage();
	// rl = getResourceLocation(m_request.m_headerFieldsMap["host"][0]);
	// Util::checkFileStat(rl.data());
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
	// generateResponse(200);
	m_parser.m_readStatus = HttpRequestParser::CONTENT;
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

	HeaderFieldsMap::const_iterator mapIt;

	for (mapIt = m_request.m_headerFieldsMap.begin();
				mapIt != m_request.m_headerFieldsMap.end(); mapIt++)
		{
			cout << "\n\t" << mapIt->first << " : ";
			vector<string>::const_iterator vecIt = mapIt->second.begin();
			for (; vecIt != mapIt->second.end(); vecIt++)
				cout << *vecIt << " ";
		}
	bool check = true;

	check &= m_request.m_headerFieldsMap.count("HOST") > 0;

	// ... check allow_method field

	if (check == false)
		throw HttpErrorHandler(400);
	// m_sendBuffer.send(m_socket->m_fd);
}

// TODO: should be called on method classes
int
RequestHandler::resolveResourceLocation(const std::string& host)
{
	string	resourceLocation;
	Tcp::SocketAddr	addr = m_socket->getAddress();
	uint64_t		addrKey = (static_cast<uint64_t>(ntohs(addr.sin_port)) << 32) + ntohl(addr.sin_addr.s_addr);
	VirtualServer*	server = ServerManager::s_virtualServerTable[addrKey][host];
	map<string, Location>&	locationTable = server->m_locationTable;


	(void)locationTable;
	return checkResourceStatus(resourceLocation.data());
//	m_method->m_resourceLocation = resourceLocation;
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
					S_IWUSR | S_IWGRP | S_IWOTH // for DELETE
//					S_IRUSR | S_IRGRP | S_IROTH // for GET, HEAD
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

// TODO: which should be first between method creation and uri checking
	statusCode = resolveResourceLocation(m_request.m_headerFieldsMap["host"][0]);
	// m_method = new AMethod(m_request, m_sendBuffer, m_recvBuffer);
	bufferResponseStatusLine(statusCode);
	bufferResponseHeaderFields();
}

void
RequestHandler::bufferResponseStatusLine(int statusCode)
{
	// status-line
	m_sendBuffer.append(g_httpVersion);
	m_sendBuffer.append(" ");
	m_sendBuffer.append(Util::toString(statusCode));
	m_sendBuffer.append(" ");
	m_sendBuffer.append(g_CRLF);
}

void
RequestHandler::bufferResponseHeaderFields()
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
