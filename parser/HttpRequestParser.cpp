#include "parser/HttpRequestParser.hpp"

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
HttpRequestParser::HttpRequestParser(std::istringstream& stream)
: m_readStatus(STATUS_LINE)
{
    m_stream.basic_ios::rdbuf(buffer.rdbuf());
}

HttpRequestParser::~HttpRequestParser()
{
}

void
HttpRequestParser::parse(HeaderFields& headerFields)
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
}

int
HttpRequestParser::checkBuffer(std::string& buffer)
{
    switch (m_readStatus)
    {
    }
}

void
HttpRequestParser::readStatusLine(const std::string& buffer)
{
}

void
HttpRequestParser::readHeaderFields(std::string& buffer)
{
}

void
HttpRequestParser::readMessageBody(std::string& buffer)
{
}
