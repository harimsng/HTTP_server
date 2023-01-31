#include <sys/stat.h>
#include <stdexcept>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "responder/Responder.hpp"
#include "http/FindLocation.hpp"
#include "parser/HttpRequestParser.hpp"
#include "http/RequestHandler.hpp"

#define CHECK_PERMISSION(mode, mask) (((mode) & (mask)) == (mask))

using namespace	std;

const char*	g_httpVersion = "HTTP/1.1";

map<string, uint16_t>	RequestHandler::s_methodConvertTable;
map<uint16_t, string>	RequestHandler::s_methodRConvertTable;
std::map<std::string, std::string>	RequestHandler::s_extensionTypeTable;


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

	switch (m_parser.m_readStatus)
	{
		case HttpRequestParser::REQUEST_LINE_METHOD:
			// fall through
		case HttpRequestParser::REQUEST_LINE:
			// fall through
		case HttpRequestParser::HEADER_FIELDS:
			m_parser.parse(m_request);
			if (m_parser.m_readStatus == HttpRequestParser::HEADER_FIELDS)
				break;
		case HttpRequestParser::HEADER_FIELDS_END:
			createResponseHeader();
			receiveStatus = RECV_EVENT;
			if (m_parser.m_readStatus == HttpRequestParser::HEADER_FIELDS_END)
				break;
		case HttpRequestParser::CONTENT:
			m_method->respond();
			if (m_parser.m_readStatus == HttpRequestParser::CONTENT)
				break;
		case HttpRequestParser::FINISHED:
			delete m_method;
			resetStates();
			break;
		default:
			;
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
	m_request = Request();
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

void
RequestHandler::checkAllowedMethod(uint16_t allowed)
{
	if (!(m_request.m_method & allowed))
		UPDATE_REQUEST_ERROR(m_request.m_status, 405);
}

// TODO: should be called on method classes

VirtualServer*
RequestHandler::resolveVirtualServer(const string& host)
{
	Tcp::SocketAddr	addr = m_socket->getAddress();
	AddrKey			addrKey;
	VirtualServer*	server;

	addrKey.setAddrKey(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
	if (ServerManager::s_virtualServerTable.count(addrKey) == 0)
		addrKey.value &= 0xffff00000000;
	if (ServerManager::s_virtualServerTable[addrKey].count(host) == 0)
		server = ServerManager::s_virtualServerTable[addrKey]["."];
	else
		server = ServerManager::s_virtualServerTable[addrKey][host];
	return server;
}

void
RequestHandler::checkResourceStatus()
{
	int			ret;
	struct stat	status;
	int			statusCode = 0;
	string		path = m_request.m_path + m_request.m_file;

	ret = stat(path.c_str(), &status);
	if (ret == 0
		&& S_ISREG(status.st_mode)
		&& CHECK_PERMISSION(status.st_mode,
//					S_IWUSR | S_IWGRP | S_IWOTH // for DELETE
					S_IRUSR | S_IRGRP | S_IROTH // for GET, HEAD
//					S_IXUSR | S_IXGRP | S_IXOTH // for POST, PUT
					))
		return;
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

	UPDATE_REQUEST_ERROR(m_request.m_status, statusCode);
}

// HOST field could be empty
void
RequestHandler::createResponseHeader() try
{
	FindLocation	findLocation;
	VirtualServer*	virtualServer;
	int&			statusCode = m_request.m_status;

	checkRequestMessage();
	virtualServer = resolveVirtualServer(m_request.m_headerFieldsMap["HOST"][0]);
	m_request.m_virtualServer = virtualServer;
	findLocation.saveRealPath(m_request, virtualServer->m_locationTable, virtualServer);
	if (m_request.m_locationBlock != NULL)
	{
		LOG(DEBUG, "location = %s", m_request.m_locationBlock->m_path.c_str());
		checkAllowedMethod(m_request.m_locationBlock->m_limitExcept);
	}
	checkResourceStatus();

	if (statusCode >= 400)
		throw HttpErrorHandler(statusCode);
	switch (m_request.m_method)
	{
		case GET:
			m_method = new GetResponder(*this);
			break;
		case HEAD:
			m_method = new HeadResponder(*this);
			break;
		case POST:
			m_method = new PostResponder(*this);
			break;
		case PUT:
			m_method = new PutResponder(*this);
			break;
		case DELETE:
			m_method = new DeleteResponder(*this);
			break;
		default:
			UPDATE_REQUEST_ERROR(statusCode, 400);
	}
	bufferResponseStatusLine(statusCode);
	bufferResponseHeaderFields();
	m_parser.m_readStatus = HttpRequestParser::CONTENT;
}
catch (HttpErrorHandler& e)
{
	if (m_request.m_method != HEAD)
		m_method = new GetResponder(*this);
	else
		m_method = new HeadResponder(*this);
	m_parser.m_readStatus = HttpRequestParser::CONTENT;
	bufferResponseStatusLine(e.m_errorCode);
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
RequestHandler::findContentType(std::string& content)
{
	std::string extension;

	extension = content.substr(content.find('.') + 1);
	if (s_extensionTypeTable.count(extension) == 1)
		return s_extensionTypeTable[extension];
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
	s_extensionTypeTable["html"] = "text/html";
	s_extensionTypeTable["htm"] = "text/html";
	s_extensionTypeTable["shtml"] = "text/html";
	s_extensionTypeTable["css"] = "text/css";
	s_extensionTypeTable["xml"] = "text/xml";
	s_extensionTypeTable["gif"] = "image/gif";
	s_extensionTypeTable["jpeg"] = "image/gif";
	s_extensionTypeTable["jpg"] = "image/jpeg";
	s_extensionTypeTable["txt"] = "text/plain";
	s_extensionTypeTable["png"] = "image/png";
	s_extensionTypeTable["ico"] = "image/x-icon";
	s_extensionTypeTable["bmp"] = "image/x-ms-bmp";
	s_extensionTypeTable["svg"] = "image/x-ms-bmp";
	s_extensionTypeTable["webp"] = "image/webp";
	s_extensionTypeTable["mp4"] = "video/mp4";
	s_extensionTypeTable["mpeg"] = "video/mp4";
	s_extensionTypeTable["mpg"] = "video/mpeg";
	s_extensionTypeTable["avi"] = "video/x-msvideo";
	s_extensionTypeTable["js"] = "application/javascript";
	s_extensionTypeTable["woff"] = "application/font-woff";
	s_extensionTypeTable["json"] = "application/json";
	s_extensionTypeTable["doc"] = "application/msword";
	s_extensionTypeTable["pdf"] = "application/pdf";
	s_extensionTypeTable["xls"] = "application/vnd.ms-excel";
	s_extensionTypeTable["rar"] = "application/x-rar-compressed";
	s_extensionTypeTable["zip"] = "application/zip";
	s_extensionTypeTable["7z"] = "application/x-7z-compressed";
	s_extensionTypeTable["bin"] = "application/zip";
	s_extensionTypeTable["exe"] = "application/zip";
	s_extensionTypeTable["mp3"] = "audio/mpeg";
	s_extensionTypeTable["ogg"] = "audio/ogg";
	s_extensionTypeTable["m4a"] = "audio/x-m4a";
}

string
RequestHandler::makeErrorPage(int status)
{
	string buf;
	string statusStr = Util::toString(status);

	buf =
	"<!DOCTYPE html>\n"
	"<html>\n"
	"	<head>\n"
	"<meta charset=\"utf-8\">\n"
	"		<title>" + statusStr + "</title>\n"
	"	</head>\n"
	"	<body>\n"
	"		<h1>"
	+ statusStr + " " + HttpErrorHandler::getErrorMessage(status) +
	"		</h1>\n"
	"	</body>\n"
	"</html>\n";

	return buf;
}
