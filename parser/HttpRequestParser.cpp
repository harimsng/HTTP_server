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
:	m_readStatus(STATUS_LINE),
	m_buffer(&buffer)
{
	m_tokenizer.init(&buffer);
}

HttpRequestParser::~HttpRequestParser()
{
}

void
HttpRequestParser::parse(HeaderFields& headerFields)
{
	std::size_t	pos;

	pos = checkBuffer(m_buffer);
	if (pos == string::npos)
		throw HttpErrorHandler(501);

	while (pos != string::npos)
	{
		switch (m_readStatus)
		{
			case STATUS_LINE:
				readStatusLine();
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
		pos = checkBuffer(m_buffer);
	}
	m_buffer->erase(0, pos + 2);
}

void
HttpRequestParser::readStatusLine()
{
}

void
HttpRequestParser::readHeaderFields()
{
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
		case STATUS_LINE:
		case HEADER_FIELDS:
			return buffer.find("\r\n");
		case MESSAGE_BODY:
			return buffer.size() - 1;
		case FINISHED:
			throw HttpErrorHandler(413);
		default:
			;
	}
}
