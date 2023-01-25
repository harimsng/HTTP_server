#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <vector>
#include <map>

#include "parser/AParser.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"
#include "Location.hpp"

class	Server;
struct	Request;

typedef std::map<std::string, std::vector<std::string> > HeaderFieldsMap;

struct Request
{
	int					m_method;
	std::string			m_uri;
	std::string			m_protocol;
	std::string			m_path;
	std::string			m_file;
	Location			m_locationBlock;
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
