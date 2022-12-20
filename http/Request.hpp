#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <vector>
#include <istream>

#include "parser/HttpRequestParser.hpp"

/*
 * RFC 9112 HTTP/1.1
 */
class	Request
{
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
		DELETE,
//		CONNECT,
//		OPTION,
//		TRACE
	};

public:
// constructors & destructor
	Request();
	~Request();

// operators

// member functions
	void	receiveData(int fd, int eventInfo);
	bool	checkStream(std::string& buffer);

// member variables
	std::istringstream	m_stream;
	HttpRequestParser	m_parser;

	int					m_method;
	std::string			m_target;
	std::string			m_protocol;
	std::map<std::string, std::vector<std::string> >	m_headerFieldsMap;
};

#endif
