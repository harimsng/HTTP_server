#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <string>
#include <map>
#include <vector>

#include "parser/HttpRequestParser.hpp"
#include "socket/Socket.hpp"
#include "io/Buffer.hpp"

extern const char*	g_httpVersion;

class	AResponder;
class	VirtualServer;

/*
 * reference: RFC 9112 HTTP/1.1
 */
class	RequestHandler
{
	friend class	HttpRequestParser;
	friend class	AResponder;

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

	enum	e_ioStatus
	{
		RECV_ERROR = 0,
		RECV_NORMAL,
		RECV_SKIPPED, 
		RECV_EVENT,
		RECV_DONE,
		SEND_ERROR,
		SEND_NORMAL,
		SEND_END
	};

// constructors & destructor
	RequestHandler(const Socket<Tcp>& socket);
	~RequestHandler();

// member functions
	int		receiveRequest();
	int		sendResponse();
	void	resetStates();

	std::string	findContentType(std::string& content);
	void	checkRequestMessage();
private:
	VirtualServer*	resolveVirtualServer(const std::string& host);
	int				resolveResourceLocation(std::map<std::string, Location>& locationTable);

	void	createResponseHeader();
	void	checkResourceStatus();
	void	checkStatusLine();
	void	checkHeaderFields();
	void	checkAllowedMethod(uint16_t allowed);
	void	checkIsCgi();

	void	makeErrorResponse(const std::string& errorMessage);

	static std::string methodToString(uint16_t allowed);

// member variables
	HttpRequestParser	m_parser;
	Request				m_request;
	AResponder*			m_responder;

public:
	const Socket<Tcp>*	m_socket;
	Buffer				m_recvBuffer;
	Buffer				m_sendBuffer;

	static std::string			makeErrorPage(int status);
// static members
	static std::map<std::string, std::string>	s_extensionTypeTable;
	static void		initExtensionList();

	static std::map<std::string, uint16_t>	s_methodConvertTable;
	static std::map<uint16_t, std::string>	s_methodRConvertTable;
	static void								setMethodConvertTable();
};

std::ostream&
operator<<(std::ostream& os, const Request& request);

#endif
