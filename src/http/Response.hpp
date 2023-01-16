#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

#include "Webserv.hpp"
#include "socket_/Socket.hpp"
#include "http/HttpInfo.hpp"
#include "http/Request.hpp"

class Server;

class	Response
{
public:
// constructors & destructor
	Response(Request& request);
	Response(const Socket<Tcp>& socket, HttpInfo& httpInfo);
	~Response();
	Response(const Response& response);

// operator
	Response	&operator=(const Response& response);

// member functions
	void		sendResponse();
	void		makeResponse();
	void		makeResponseHeader();
	void		makeResponseBody();
	void		makeErrorResponse(const std::string& errorMessage);

	std::string	getResourceLocation(const std::string& host);

private:
// member variables
	Request*			m_request;
	std::string			m_buffer;

	HttpInfo*			m_httpInfo;
};

#endif
