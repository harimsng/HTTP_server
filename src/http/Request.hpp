#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <vector>

#include "Location.hpp"
#include "parser/HttpRequestParser.hpp"
#include "socket_/Socket.hpp"
#include "http/HttpInfo.hpp"

/*
 * reference: RFC 9112 HTTP/1.1
 */
class	Request
{
	friend class	HttpRequestParser;
	friend class	Response;

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
	Request(const Socket<Tcp>& socket, HttpInfo& httpInfo);
	~Request();

// member functions
	int		receiveRequest();
private:
	int		receiveRawData();

// member variables
	const Socket<Tcp>*	m_socket;
	std::string			m_buffer;
	HttpRequestParser	m_parser;

	HttpInfo*			m_httpInfo;

	/* NOTE
	 * 아래의 변수들은 이제 m_httpInfo에 저장되어있음
	 * */
	int					m_method;
	std::string			m_uri;
	std::string			m_protocol;
	HeaderFieldsMap		m_headerFieldsMap;
};

#endif
