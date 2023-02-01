#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <vector>
#include <map>

#include "parser/AParser.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"
#include "Location.hpp"

#define UPDATE_REQUEST_ERROR(status, code) (status = status < 300 ? code : status)

class	Server;
class	VirtualServer;
struct	Request;

typedef std::map<std::string, std::vector<std::string> > HeaderFieldsMap;

struct Request
{
	Request(): m_status(200), m_locationBlock(NULL), m_virtualServer(NULL) {};

	int					m_method;
	int					m_status;
	std::string			m_uri;
	std::string			m_queryString;
	std::string			m_protocol;
	std::string			m_path;
	std::string			m_file;
	std::string			m_cgi;
	Location*			m_locationBlock;
	VirtualServer*		m_virtualServer;
	HeaderFieldsMap		m_headerFieldsMap;
};

class	HttpRequestParser
:	public AParser<HttpStreamTokenizer, Request>
{
// deleted
	HttpRequestParser(const HttpRequestParser& parser);
	HttpRequestParser&	operator=(const HttpRequestParser& parser);

// types
public:
	enum	e_readStatus
	{
		REQUEST_LINE_METHOD = 0,
		REQUEST_LINE,
		HEADER_FIELDS,
		HEADER_FIELDS_END,
		CONTENT,
		FINISHED,
	};

// constructors & destructor
	HttpRequestParser(std::string& buffer);
	~HttpRequestParser();

// member functions
	virtual void			parse(Request& request);
	std::string::size_type	updateBuffer();

	void	parseMethod(Request& request);
	void	parseStatusLine(Request& request);
	void	parseHeaderFields(HeaderFieldsMap& headerFieldsMap);

	e_readStatus	checkStatusLine(Request& request);
	bool			checkHeaderFields(HeaderFieldsMap& headerFieldsMap);

// member variables
	e_readStatus		m_readStatus;
};

#endif
