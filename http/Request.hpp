#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <vector>

#include "parser/HttpRequestParser.hpp"
#include "socket_/Socket.hpp"

/*
 * RFC 9112 HTTP/1.1
 */
class	Request
{
	friend class	HttpRequestParser;

// deleted
	Request(const Request& request);
	Request	&operator=(const Request& request);

// types
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

public:
// constructors & destructor
	Request(const Socket<Tcp>& socket);
	~Request();

// member functions
	int		receiveRequest(int eventInfo);
private:
	int		receiveRawData(int eventInfo);

// member variables
	const Socket<Tcp>*	m_socket;
	std::string			m_buffer;
	HttpRequestParser	m_parser;

	int					m_method;
	std::string			m_target;
	std::string			m_protocol;
	HeaderFieldsMap		m_headerFieldsMap;
};

#endif
