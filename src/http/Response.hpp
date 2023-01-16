#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

#include "socket_/Socket.hpp"
#include "http/HttpInfo.hpp"

class Server;

class	Response
{
// deleted
	Response(const Response& response);
	Response	&operator=(const Response& response);

public:
// constructors & destructor
	Response(const Socket<Tcp>& socket);
	Response(const Socket<Tcp>& socket, HttpInfo& httpInfo);
	~Response();

// member functions
	void		sendResponse();
	void		makeErrorResponse(const std::string& errorMessage);

private:

// member variables
	const Socket<Tcp>*	m_socket;
	std::string			m_buffer;

	HttpInfo*			m_httpInfo;
};

#endif
