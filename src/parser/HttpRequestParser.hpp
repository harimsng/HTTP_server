#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <vector>
#include <map>

#include "parser/AParser.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"

class	Server;
struct	Request;

typedef std::map<std::string, std::vector<std::string> > HeaderFieldsMap;

struct Request
{
	int					m_method;
	std::string			m_uri;
	std::string			m_protocol;
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
		REQUEST_LINE = 0,
		HEADER_FIELDS,
		HEADER_FIELDS_END,
		MESSAGE_BODY,
		FINISHED
	};

// constructors & destructor
	HttpRequestParser(std::string& buffer);
	~HttpRequestParser();

// member functions
	virtual void			parse(Request& request);
	std::string::size_type	updateBuffer();

	void	parseStatusLine(Request& request);
	void	parseHeaderFields(HeaderFieldsMap& headerFieldsMap);
	void	parseMessageBody(Request& request);

	e_readStatus	checkStatusLine(Request& request);
	bool			checkHeaderFields(HeaderFieldsMap& headerFieldsMap);

	e_readStatus	getReadStatus() const;

// member variables
	e_readStatus		m_readStatus;
};

#endif
