#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include "parser/AParser.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"

struct	Request;

#define UPDATE_REQUEST_ERROR(status, code) (status = status < 300 ? code : status)

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
		ERROR
	};

// constructors & destructor
	HttpRequestParser(std::string& buffer);
	~HttpRequestParser();

// member functions
	virtual void			parse(Request& request);
	std::string::size_type	updateBuffer();

	void	parseMethod(Request& request);
	void	parseStatusLine(Request& request);
	void	parseHeaderFields(Request& request);

// member variables
	e_readStatus		m_readStatus;
};

#endif
