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

const char*	g_httpVersion = "HTTP/1.1";

map<string, uint16_t>	RequestHandler::s_methodConvertTable;
map<uint16_t, string>	RequestHandler::s_methodRConvertTable;
vector<pair<string, string> >	RequestHandler::s_extensionTypeTable;


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
RequestHandler::receiveRequest()
{
	int		count;
	int		receiveStatus = RECV_NORMAL;

	if (m_sendBuffer.size() != 0)
		return RECV_SKIPPED;

	count = m_recvBuffer.receive(m_socket->m_fd);
	if (count == 0)
		return RECV_END;
	else if (count == -1)
		return RECV_SKIPPED;

	if (m_parser.m_readStatus < HttpRequestParser::HEADER_FIELDS_END)
		m_parser.parse(m_request);
	if (m_parser.m_readStatus == HttpRequestParser::HEADER_FIELDS_END)
	{
		createResponseHeader();
		receiveStatus = RECV_EVENT;
	}
	if (m_parser.m_readStatus == HttpRequestParser::CONTENT)
	{
		// NOTE
		// this method will be called multiple times. this block is temporary.
		m_method->completeResponse();
		// resetStates();
	}
	if (m_parser.m_readStatus == HttpRequestParser::FINISHED)
	{
		delete m_method;
		resetStates();
	}
	return receiveStatus;
}

int
RequestHandler::sendResponse() try
{
	int		count = m_sendBuffer.send(m_socket->m_fd);

	if (count == 0 && m_parser.m_readStatus == HttpRequestParser::REQUEST_LINE_METHOD)
	{
		return SEND_DONE;
	}
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

	// if (m_request.m_uri == "/")
	//     throw HttpErrorHandler(400);
	checkStatusLine(); // 1, 2, 3
	checkHeaderFields(); // 4
}

void
RequestHandler::checkStatusLine()
{
	// if (m_request.m_uri >= uri_size) // check uri length
	if (m_request.m_protocol != "HTTP/1.1") // check http version
		UPDATE_REQUEST_ERROR(m_request.m_status, 505);
}

void
RequestHandler::checkHeaderFields()
{
	bool check = true;

	check &= m_request.m_headerFieldsMap.count("HOST") > 0;

	if (check == false)
		UPDATE_REQUEST_ERROR(m_request.m_status, 400);
}

bool
RequestHandler::checkAllowedMethod(uint16_t allowed)
{
	if (!(m_request.m_method & allowed))
		throw HttpErrorHandler(405);
	return true;
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
	int&			statusCode = m_request.m_status;

	checkRequestMessage();
	virtualServer = resolveVirtualServer(m_request.m_headerFieldsMap["HOST"][0]);
	m_request.m_virtualServer = virtualServer;
	resourceLocation = findLocation.saveRealPath(m_request, virtualServer->m_locationTable, virtualServer);
	if (m_request.m_locationBlock != NULL)
		checkAllowedMethod(m_request.m_locationBlock->m_limitExcept);
	statusCode = checkResourceStatus(resourceLocation.c_str());

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
		default:
			UPDATE_REQUEST_ERROR(statusCode, 400);
	}
	bufferResponseStatusLine(statusCode);
	bufferResponseHeaderFields();
	if (statusCode >= 400)
		throw HttpErrorHandler(statusCode);

	m_parser.m_readStatus = HttpRequestParser::CONTENT;
}
catch (HttpErrorHandler& e)
{
	if (m_request.m_method != HEAD)
		m_method = new GetMethod(*this);
	else
		m_method = new HeadMethod(*this);
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
	m_sendBuffer.append(HttpErrorHandler::getErrorMessage(statusCode));
	m_sendBuffer.append(g_CRLF);
}

void
RequestHandler::bufferResponseHeaderFields()
{
	m_sendBuffer.append("Server: webserv/2.0");
	m_sendBuffer.append(g_CRLF);
	// TODO : should be moved to method class
	// m_sendBuffer.append("Content-Type: " + findContentType(m_request.m_file));
	// m_sendBuffer.append(g_CRLF);
	// m_sendBuffer.append("Connection: keep-alive");
	// m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append("Date: " + Util::getDate("%a, %d %b %Y %X %Z"));
	m_sendBuffer.append(g_CRLF);
}


std::string
RequestHandler::findContentType(std::string content)
{
	std::string extension;

	extension = content.substr(content.find('.') + 1);
	for (std::vector<std::pair<std::string, std::string> >::const_iterator it = s_extensionTypeTable.begin(); it != s_extensionTypeTable.end(); it++)
		if (extension == it->first)
			return (it->second);
	extension = "text/html";
	return (extension);
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

	s_methodRConvertTable[RequestHandler::GET] = "GET";
	s_methodRConvertTable[RequestHandler::HEAD] = "HEAD";
	s_methodRConvertTable[RequestHandler::POST] = "POST";
	s_methodRConvertTable[RequestHandler::PUT] = "PUT";
	s_methodRConvertTable[RequestHandler::DELETE] = "DELETE";
	s_methodRConvertTable[RequestHandler::OPTION] = "OPTION";
	s_methodRConvertTable[RequestHandler::TRACE] = "TRACE";
	s_methodRConvertTable[RequestHandler::CONNECT] = "CONNECT";
}

void
RequestHandler::initExtensionList()
{
	s_extensionTypeTable.reserve(64);
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("html", "text/html"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("htm","text/html"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("shtml","text/html"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("css","text/css"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("xml","text/xml"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("gif","image/gif"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("jpeg","image/gif"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("jpg","image/jpeg"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("txt","text/plain"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("png","image/png"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("ico","image/x-icon"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("bmp","image/x-ms-bmp"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("svg","image/x-ms-bmp"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("webp","image/webp"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("mp4", "video/mp4"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("mpeg", "video/mp4"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("mpg", "video/mpeg"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("avi", "video/x-msvideo"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("js","application/javascript"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("woff","application/font-woff"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("json","application/json"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("doc","application/msword"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("pdf","application/pdf"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("xls", "application/vnd.ms-excel"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("rar", "application/x-rar-compressed"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("zip", "application/zip"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("7z", "application/x-7z-compressed"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("bin", "application/zip"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("exe", "application/zip"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("mp3", "audio/mpeg"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("ogg", "audio/ogg"));
	s_extensionTypeTable.push_back(std::pair<std::string, std::string>("m4a", "audio/x-m4a"));
}
