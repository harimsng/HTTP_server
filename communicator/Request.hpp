#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <sstream>
#include <map>
#include <vector>

class HttpRequestParser;
class Communicator;

class Request
{
public:

	typedef std::map<std::string, std::vector<std::string> > requestHeaderMap;

	enum requestSection
	{
		REQUEST_LINE = 0,
		REQUEST_HEADER,
		REQUEST_HEADER_END,
		REQUEST_BODY,
		REQUEST_END
	};

public:
	Request();
	~Request();

	int		makeRequestLine(const std::string& buffer);
	int		makeRequestHeader(const std::string& buffer);
	void	makeReqeustBody(const std::string& buffer);
	void	makeRequest(std::string& buffer);
	void	printRequestMessage() const;
	int&	getRequestSection();

	std::string			m_methodType;
	std::string			m_uri;
	int					m_requestSection;
	int					m_preBufferSize;

	requestHeaderMap	m_requestHeaderMap;
};

#endif //Request_hpp
