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

map<string, uint16_t>	RequestHandler::s_methodConvertTable;

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
	int		count;
	int		receiveStatus = RECV_NORMAL;

	if (m_sendBuffer.size() != 0)
		return RECV_SKIPPED;

	count = m_recvBuffer.receive(m_socket->m_fd);
	if (count == 0)
		return RECV_END;
	else if (count == -1)
		throw HttpErrorHandler(500);

	if (m_parser.m_readStatus < HttpRequestParser::HEADER_FIELDS_END)
	{
		m_parser.parse(m_request);
	}
	if (m_parser.m_readStatus == HttpRequestParser::HEADER_FIELDS_END)
	{
		//m_method->createResponseHeader();
		createResponseHeader();
		receiveStatus = RECV_EVENT;
	}
	if (m_parser.m_readStatus == HttpRequestParser::CONTENT)
	{
		//m_method->createResponseContent

		// this method will be called multiple times. this block is temporary.
		m_method->completeResponse();
	}
	return receiveStatus;
}
catch (HttpErrorHandler& e)
{
	LOG(ERROR, "%s", e.getErrorMessage().data());
	m_sendBuffer.append("HTTP/1.1 301 Moved Permanently");
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append("Location: http://localhost:8080/error.html");
	m_sendBuffer.append(g_CRLF);
	// m_sendBuffer.append(gtCRLF);
	// bufferResponseStatusLine(400);
	// bufferResponseHeaderFields();
	resetStates();
	return (RECV_EVENT);
}

int
RequestHandler::sendResponse() try
{
	int		count = m_sendBuffer.send(m_socket->m_fd);
	
	if (count == 0 && m_parser.m_readStatus == HttpRequestParser::REQUEST_LINE_METHOD)
		return SEND_DONE;
	return SEND_NORMAL;
}
catch (runtime_error& e)
{
	LOG(ERROR, "%s", e.what());
	return SEND_ERROR;
}

void
RequestHandler::resetStates()
{
	m_request.m_headerFieldsMap.clear();
	m_parser.m_readStatus = HttpRequestParser::REQUEST_LINE_METHOD;
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

//	if (m_request.m_uri == "/")
//		throw HttpErrorHandler(400);
	checkStatusLine(); // 1, 2, 3
	checkHeaderFields(); // 4
}

void
RequestHandler::checkStatusLine()
{
	if (m_request.m_method == METHOD_ERROR) // check method
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

bool
RequestHandler::checkAllowedMethod(uint16_t allowed)
{
	return m_request.m_method & allowed;
}

// TODO: should be called on method classes

VirtualServer*
RequestHandler::resolveVirtualServer(const string& host)
{
	Tcp::SocketAddr	addr = m_socket->getAddress();
	uint64_t		addrKey = Util::convertAddrKey(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
	VirtualServer*	server;

	if (ServerManager::s_virtualServerTable.count(addrKey) == 0)
		addrKey &= 0xffff00000000;
	if (ServerManager::s_virtualServerTable[addrKey].count(host) == 0)
		server = ServerManager::s_virtualServerTable[addrKey]["."];
	else
		server = ServerManager::s_virtualServerTable[addrKey][host];
	return server;
}

int
RequestHandler::checkResourceStatus(const char* path)
{
	int			ret;
	struct stat	status;
	int			statusCode = 0;

	ret = stat(path, &status);
	if (ret == 0
		&& S_ISREG(status.st_mode)
		&& CHECK_PERMISSION(status.st_mode,
//					S_IWUSR | S_IWGRP | S_IWOTH // for DELETE
					S_IRUSR | S_IRGRP | S_IROTH // for GET, HEAD
//					S_IXUSR | S_IXGRP | S_IXOTH // for POST, PUT
					))
		return (200);
		// statusCode = 200;

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
	LOG(WARNING, "couldn't find requested resource. status Code = %d", statusCode);
	return statusCode;
}

// HOST field could be empty
void
RequestHandler::createResponseHeader() try
{
	FindLocation	findLocation;
	VirtualServer*	virtualServer;
	string			resourceLocation;
	int				statusCode = m_request.m_status;

	checkRequestMessage();
	virtualServer = resolveVirtualServer(m_request.m_headerFieldsMap["HOST"][0]);
	resourceLocation = findLocation.saveRealPath(m_request, virtualServer->m_locationTable, virtualServer);
	checkAllowedMethod(m_request.m_locationBlock.m_limitExcept);
	checkResourceStatus(resourceLocation.c_str());
	if (statusCode >= 400)
		throw HttpErrorHandler(statusCode);
	bufferResponseStatusLine(statusCode);
	bufferResponseHeaderFields();
	switch (m_request.m_method)
	{
		case GET:
			m_method = new GetMethod(*this);
			break;
		case HEAD:
			m_method = new HeadMethod(*this);
			break;
		case POST:
			m_method = new PostMethod(*this);
			break;
		case PUT:
			m_method = new PutMethod(*this);
			break;
		case DELETE:
			m_method = new DeleteMethod(*this);
			break;
		default: ;
			// throw HttpErrorHandler(???);
	}
	m_parser.m_readStatus = HttpRequestParser::CONTENT;
}
catch (HttpErrorHandler& e)
{
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

void
RequestHandler::setMethodConvertTable()
{
	s_methodConvertTable["GET"] = RequestHandler::GET;
	s_methodConvertTable["HEAD"] = RequestHandler::HEAD;
	s_methodConvertTable["POST"] = RequestHandler::POST;
	s_methodConvertTable["PUT"] = RequestHandler::PUT;
	s_methodConvertTable["DELETE"] = RequestHandler::DELETE;
	s_methodConvertTable["OPTION"] = RequestHandler::OPTION;
	s_methodConvertTable["TRACE"] = RequestHandler::TRACE;
	s_methodConvertTable["CONNECT"] = RequestHandler::CONNECT;
}
