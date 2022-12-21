#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <vector>
#include <map>
#include <sstream>

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
	HttpRequestParser(std::string& buffer);
	~HttpRequestParser();

// member functions
	virtual void			parse(HeaderFieldsMap& headerFields);
	std::string::size_type	checkBuffer(std::string* buffer);

	void	readStatusLine();
	void	readHeaderFields();
	void	readMessageBody();

	e_readStatus	getReadStatus() const;

// member variables
private:
	e_readStatus		m_readStatus;
	std::string*		m_buffer;
};

#endif
