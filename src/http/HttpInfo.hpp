#ifndef HTTPINFO_HPP
#define HTTPINFO_HPP

#include <string>
#include <vector>
#include <map>
#include <ostream>

class Server;

typedef std::map<std::string, std::vector<std::string> > requestHeaderFields_t;

class HttpInfo
{
// deleted

public:
	enum e_requestReadStatus
	{
		REQUEST_LINE = 0,
		HEADER_FIELDS,
		MESSAGE_BODY,
		FINISHED
	};
	enum e_method
	{
		GET = 1,
		HEAD,
		POST,
		PUT,
		DELETE
	};
// constructors & destructor
	HttpInfo();
	~HttpInfo();

// operator
	friend std::ostream&	operator<<(std::ostream& os, const HttpInfo& httpInfo);

	// const Server*			m_server;
	int						m_method;
	std::string				m_target;
	std::string				m_protocol;
	int						m_requestReadStatus;
	requestHeaderFields_t	m_requestHeaderFields;

};
#endif //HttpInfo_hpp
