#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <string>
#include <map>
#include <vector>

#include "Location.hpp"
#include "parser/HttpRequestParser.hpp"
#include "socket/Socket.hpp"

/*
 * reference: RFC 9112 HTTP/1.1
 */
class	RequestHandler
{
	friend class	HttpRequestParser;
	friend class	Response;

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
		DELETE
//		CONNECT,
//		OPTION,
//		TRACE
	};

// constructors & destructor
	RequestHandler(const Socket<Tcp>& socket);
	~RequestHandler();

// member functions
	int		receiveRequest();
	void	makeResponse();
	void	sendResponse();

	std::string	getResourceLocation(const std::string& host);

private:
	int		receiveRawData();
	void	makeResponseHeader();
	void	makeResponseBody();
	void	makeErrorResponse(const std::string& errorMessage);

// member variables
	const Socket<Tcp>*	m_socket;
	std::string			m_recvBuffer;
	std::string			m_sendBuffer;
	HttpRequestParser	m_parser;

	Request				m_request;
};

#endif
