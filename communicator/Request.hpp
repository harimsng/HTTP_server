#ifndef Request_hpp
#define Request_hpp

#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

class HttpRequestParser;
class Communicator;

class Request
{
public:

	typedef std::unordered_map<std::string, std::vector<std::string> > requestHeaderMap;

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
	int		makeRequest(std::string& buffer);
	bool	checkUri(void);
	void	printRequestMessage(requestHeaderMap& requestHeaderMap) const;

	requestHeaderMap	m_requestHeaderMap;
	std::string			m_methodType;
	std::string			m_uri;
	int					m_requestSection;
	int					m_preBuffersize;

};
#endif //Request_hpp
