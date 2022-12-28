#include "exception/HttpErrorHandler.hpp"
#include "http/Request.hpp"
#include "parser/HttpRequestParser.hpp"

using namespace std;

// deleted
HttpRequestParser::HttpRequestParser(const HttpRequestParser& parser)
{
    (void)parser;
}

HttpRequestParser&
HttpRequestParser::operator=(const HttpRequestParser& parser)
{
    (void)parser;
    return *this;
}

// constructors & destructor
HttpRequestParser::HttpRequestParser(std::string& buffer)
:	m_readStatus(REQUEST_LINE)
{
	m_tokenizer.init(buffer);
}

HttpRequestParser::~HttpRequestParser()
{
}

void
HttpRequestParser::parse(Request& request)
{
	std::size_t	pos;

	(void)pos;
	if (m_tokenizer.initBuffer() == false)
		throw HttpErrorHandler(501);

	while (m_tokenizer.empty() == false)
	{
		switch (m_readStatus)
		{
			case REQUEST_LINE:
				readStatusLine(request);
			case HEADER_FIELDS:
				break;
			case MESSAGE_BODY:
				break;
			case FINISHED:
				break;
			default:
				throw std::logic_error("unhandled read status in \
HttpRequestParser::preprocess()");
		}
	}
}

void
HttpRequestParser::readStatusLine(Request& request)
{
	(void)request;
	while (m_tokenizer.getc() != '\n')
	{
	}

	// int spacePos;
	// string token = m_tokenizer.get();
    //
	// spacePos = token.find(" ", 0);
	// token.substr(0, spacePos);
}

void
HttpRequestParser::readHeaderFields(HeaderFieldsMap& headerFieldsMap)
{
	(void)headerFieldsMap;
}

void
HttpRequestParser::readMessageBody()
{
}

HttpRequestParser::e_readStatus
HttpRequestParser::getReadStatus() const
{
    return m_readStatus;
}

string::size_type
HttpRequestParser::checkBuffer(std::string* buffer)
{
	switch (m_readStatus)
	{
		case REQUEST_LINE:
		case HEADER_FIELDS:
			return buffer->find("\r\n");
		case MESSAGE_BODY:
			return buffer->size() - 1;
		case FINISHED:
			throw HttpErrorHandler(413);
		default:
			;
	}
}
