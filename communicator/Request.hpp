#ifndef Request_hpp
#define Request_hpp

#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

class HttpRequestParser;
class Communicator;

class Request
{
public:

	typedef map<string, vector<string> > requestHeaderMap;

	enum requestSection
	{
		REQUEST_LINE = 0,
		REQUEST_HEADER,
		REQUEST_BODY,
		REQUEST_END
	};
private:
	string	m_methodType;
	string	m_uri;
	int		m_requestSection;

public:
	Request();
	~Request();

	int		getRequestSection() const;

	int		makeRequestLine(const string& buffer);
	int		makeRequestHeader(const string& buffer, requestHeaderMap& requestHeaderMap);
	void	makeReqeustBody(const string& buffer);
	bool	checkUri(void);

};
#endif //Request_hpp
