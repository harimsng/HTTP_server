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
	string::size_type	pos;

	pos = updateBuffer();
	if (pos == string::npos)
		return;

	while (m_tokenizer.empty() == false)
	{
		switch (m_readStatus)
		{
			case REQUEST_LINE:
				readStatusLine(request);
				break;
			case HEADER_FIELDS:
				readHeaderFields(request.m_headerFieldsMap);
				break;
			case MESSAGE_BODY:
				// normal transfer or chunked
				readMessageBody();
				break;
			case FINISHED:
				// trailer section is not implemented
				throw HttpErrorHandler(501);
				break;
			default:
				throw std::logic_error("unhandled read status in \
HttpRequestParser::parse()");
		}
	}
}

string::size_type
HttpRequestParser::updateBuffer()
{
	if (m_readStatus < MESSAGE_BODY)
		return m_tokenizer.updateBufferForHeader();
	else
	 	return m_tokenizer.updateBufferForBody();
}

void
HttpRequestParser::readStatusLine(Request& request)
{
	const string&	line = m_tokenizer.getline();

	(void)request;
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
