#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <string>
#include <map>
#include <vector>

#include "Location.hpp"
#include "parser/HttpRequestParser.hpp"
#include "socket/Socket.hpp"
#include "io/Buffer.hpp"

const std::string	g_httpVersion = "HTTP/1.1";

class	AMethod;

/*
 * reference: RFC 9112 HTTP/1.1
 */
class	RequestHandler
{
	friend class	HttpRequestParser;

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

// constructors & destructor
	RequestHandler(const Socket<Tcp>& socket);
	~RequestHandler();

// member functions
	int		receiveRequest();
	void	generateResponse(int statusCode);
	void	sendResponse();

	std::string	getResourceLocation(const std::string& host);
	// friend std::ostream& operator<<(std::ostream& os, const Request& request);

private:
	void	makeResponseHeader();
	void	makeResponseBody();
	void	makeErrorResponse(const std::string& errorMessage);
	void	checkRequestMessage();
	void	checkStatusLine();
	void	checkHeaderFields();

// member variables
	const Socket<Tcp>*	m_socket;
	ReceiveBuffer		m_recvBuffer;
	SendBuffer			m_sendBuffer;
	HttpRequestParser	m_parser;

	Request				m_request;
	AMethod*			m_method;
};

#endif
