#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <vector>
#include <map>
#include <string>

class	Server;
class	Location;
class	VirtualServer;

typedef std::map<std::string, std::vector<std::string> > HeaderFieldsMap;

struct Request
{
	Request(): m_method(0), m_status(200), m_locationBlock(NULL), m_virtualServer(NULL), m_isCgi(false) {};

	int					m_method;
	int					m_status;
	std::string			m_uri;
	std::string			m_queryString;
	std::string			m_protocol;
	std::string			m_path;
	std::string			m_file;
	std::string			m_cgi;
	std::string			requestBodyBuf;
	Location*			m_locationBlock;
	VirtualServer*		m_virtualServer;
	HeaderFieldsMap		m_headerFieldsMap;
	bool				m_isCgi;
};

#endif
