#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <vector>
#include <sstream>

#include "parser/AParser.hpp"
#include "tokenizer/SocketStreamTokenizer.hpp"

class	Request;

typedef std::vector<std::string, std::vector<std::string> >	HeaderFields;

class	HttpRequestParser
:	public AParser<SocketStreamTokenizer, HeaderFields>
{
// deleted
	HttpRequestParser(const HttpRequestParser& parser);
	HttpRequestParser&	operator=(const HttpRequestParser& parser);

// types
	enum	e_readStatus
	{
		STATUS_LINE = 0,
		HEADER_FIELDS,
		MESSAGE_BODY,
		FINISHED
	};

public:
// constructors & destructor
	HttpRequestParser(std::stringstream& buffer);
	~HttpRequestParser();

// member functions
	virtual void	parse(HeaderFields& headerFields);
	int				checkBuffer(std::string& buffer);

	void	readStatusLine(std::string& buffer);
	void	readHeaderFields(std::string& buffer);
	void	readMessageBody(std::string& buffer);

// member variables
	e_readStatus		m_readStatus;
	std::stringstream	m_stream;
};

#endif
