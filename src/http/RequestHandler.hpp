#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <string>
#include <map>
#include <vector>

#include "Location.hpp"
#include "parser/HttpRequestParser.hpp"
#include "socket/Socket.hpp"
#include "io/Buffer.hpp"

static const char*	g_httpVersion = "HTTP/1.1";

class	AMethod;

/*
 * reference: RFC 9112 HTTP/1.1
 */
class	RequestHandler
{
	friend class	HttpRequestParser;
	friend class	AMethod;

// deleted
	RequestHandler(const RequestHandler& requestHandler);
	RequestHandler	&operator=(const RequestHandler& requestHandler);

// types
public:
	enum	e_method
	{
		GET = 1,
		HEAD,
		POST,
		PUT,
		DELETE,
		ERROR
//		CONNECT,
//		OPTION,
//		TRACE
	};

	enum	e_receiveStatus
	{
		RECV_END = 0,
		RECV_ERROR = 1,
		RECV_NORMAL = 2,
		RECV_SKIPPED = 3,
		RECV_EVENT = 4,
		RECV_DONE = 5,
		SEND_ERROR = 6,
		SEND_NORMAL = 7,
		SEND_DONE = 8
	};

// constructors & destructor
	RequestHandler(const Socket<Tcp>& socket);
	~RequestHandler();

// member functions
	int		receiveRequest();
	int		sendResponse();
	void	resetStates();

	int		resolveResourceLocation(const std::string& host);

private:
	void	createResponseHeader();
	void	bufferResponseStatusLine(int statusCode);
	void	bufferResponseHeaderFields();

	int		checkResourceStatus(const char* path);
	void	checkRequestMessage();
	void	checkStatusLine();
	void	checkHeaderFields();

	void		initExtensionList();
	std::string	findContentType(std::string content);

	void	makeErrorResponse(const std::string& errorMessage);

// member variables
	const Socket<Tcp>*	m_socket;
	ReceiveBuffer		m_recvBuffer;
	SendBuffer			m_sendBuffer;
	HttpRequestParser	m_parser;

	Request				m_request;
	AMethod*			m_method;
	std::vector<std::pair<std::string, std::string> > m_extensionType;
};

std::ostream&
operator<<(std::ostream& os, const Request& request);

#endif
