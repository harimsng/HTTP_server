#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <vector>
#include <sstream>
#include <map>

#include "parser/AParser.hpp"
#include "tokenizer/SocketStreamTokenizer.hpp"

class	Request;
class	Server;

typedef std::map<std::string, std::vector<std::string> > HeaderFieldsMap;

class	HttpRequestParser
:	public AParser<SocketStreamTokenizer, HeaderFieldsMap>
{
// deleted
	HttpRequestParser(const HttpRequestParser& parser);
	HttpRequestParser&	operator=(const HttpRequestParser& parser);

// types
public:
	enum	e_readStatus
	{
		STATUS_LINE = 0,
		HEADER_FIELDS,
		MESSAGE_BODY,
		FINISHED
	};

// constructors & destructor
	HttpRequestParser();
	~HttpRequestParser();

// member functions
	virtual void	parse(HeaderFieldsMap& headerFields);
	void			push(const std::string& buffer);

	void	readStatusLine(std::string& buffer);
	void	readHeaderFields(std::string& buffer);
	void	readMessageBody(std::string& buffer);

	e_readStatus	getReadStatus() const;

// member variables
private:
	e_readStatus		m_readStatus;
};

#endif
