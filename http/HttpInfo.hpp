#ifndef HttpInfo_hpp
#define HttpInfo_hpp

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
// constructors & destructor
	HttpInfo(Server& server);
	~HttpInfo();

// operator
	friend std::ostream&	operator<<(std::ostream& os, const HttpInfo& httpInfo);

	const Server*			m_server;
	std::string				m_method;
	std::string				m_target;
	std::string				m_protocol;
	int						m_requestReadStatus;
	requestHeaderFields_t	m_requestHeaderFields;

};
#endif //HttpInfo_hpp
