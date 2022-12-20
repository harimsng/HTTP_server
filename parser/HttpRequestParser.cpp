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
HttpRequestParser::HttpRequestParser()
: m_readStatus(STATUS_LINE)
{
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

void
HttpRequestParser::push(const std::string& buffer)
{
    m_tokenizer.
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

HttpRequestParser::e_readStatus
HttpRequestParser::getReadStatus() const
{
    return m_readStatus;
}
