#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <string>
#include <map>
#include <vector>

#include "parser/HttpRequestParser.hpp"
#include "socket/Socket.hpp"
#include "io/Buffer.hpp"

static const char*	g_httpVersion = "HTTP/1.1";

class	AMethod;
class	VirtualServer;

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
		METHOD_ERROR = 0x0,
		GET = 0x1,
		HEAD = 0x2,
		POST = 0x4,
		PUT = 0x8,
		DELETE = 0x10,
		CONNECT = 0x20,
		OPTION = 0x40,
		TRACE = 0x80
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

	void	createResponseHeader();
private:
	VirtualServer*	resolveVirtualServer(const std::string& host);
	int				resolveResourceLocation(std::map<std::string, Location>& locationTable);

	int		checkResourceStatus(const char* path);
	void	checkRequestMessage();
	void	checkStatusLine();
	void	checkHeaderFields();
	bool	checkAllowedMethod(uint16_t allowed);

	void	bufferResponseStatusLine(int statusCode);
	void	bufferResponseHeaderFields();
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
public:
// static members
	static std::map<std::string, uint16_t>	s_methodConvertTable;
	static void								setMethodConvertTable();
};

std::ostream&
operator<<(std::ostream& os, const Request& request);

#endif
