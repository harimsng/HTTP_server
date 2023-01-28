#include "Logger.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "http/RequestHandler.hpp"
#include "parser/HttpRequestParser.hpp"

using namespace std;

// deleted
HttpRequestParser::HttpRequestParser(const HttpRequestParser& parser)
:	AParser<HttpStreamTokenizer, Request>()
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
:	m_readStatus(REQUEST_LINE_METHOD)
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

	pos = m_tokenizer.updateBuffer();
	if (pos == string::npos)
		return;
	while (m_tokenizer.empty() == false)
	{
		switch (m_readStatus)
		{
			case REQUEST_LINE_METHOD:
			case REQUEST_LINE:
				parseMethod(request);
				parseStatusLine(request);
				break;
			case HEADER_FIELDS:
				parseHeaderFields(request.m_headerFieldsMap);
				break;
			default:
				throw std::logic_error("unhandled read status in\
HttpRequestParser::parse()");
		}
	}
	m_tokenizer.flush();
}

void
HttpRequestParser::parseMethod(Request &request)
{
	char	c = m_tokenizer.getc();
	string	method;

	method.reserve(8);
	while (c != '\0' && c != ' ')
	{
		method += c;
		c = m_tokenizer.getc();
	}
	request.m_method = RequestHandler::s_methodConvertTable[method];
	LOG(DEBUG, "request.m_method = %x", request.m_method);
	m_readStatus = REQUEST_LINE;
}

void
HttpRequestParser::parseStatusLine(Request &request)
{
	const string	statusLine = m_tokenizer.getline();
	string			method;
	size_t			spacePos;

	LOG(DEBUG, "statusline = \"%s\"", statusLine.data());
	spacePos = statusLine.find(" ");
	if (spacePos == string::npos)
		throw HttpErrorHandler(400);
	request.m_uri = statusLine.substr(0, spacePos);
	request.m_protocol = statusLine.substr(spacePos + 1);
	LOG(DEBUG, "uri = %s, protocol = %s", request.m_uri.data(), request.m_protocol.data());
	m_readStatus = HEADER_FIELDS;
}

void
HttpRequestParser::parseHeaderFields(HeaderFieldsMap& headerFieldsMap)
{
	const string headerLine = m_tokenizer.getline();
	string	field;
	string	value;
	size_t	pos;
	size_t	curPos;

	curPos = 0;
	pos = headerLine.find(": ");
	field = Util::toUpper(headerLine.substr(curPos, pos));
	curPos = pos + 2;
	while (1)
	{
		pos = headerLine.find(") ", curPos);
		if (pos == string::npos)
			pos = headerLine.find(",", curPos);
		if (pos == string::npos)
		{
			value = headerLine.substr(curPos);
			headerFieldsMap[field].push_back(Util::toLower(value));
			break;
		}
		pos++;
		value = headerLine.substr(curPos, pos - curPos);
		curPos = headerLine[pos] == ' ' ? pos + 1 : pos;
		if (value[value.length() - 1] == ',')
			value.erase(value.end() - 1);
		headerFieldsMap[field].push_back(Util::toLower(value));
	}
	if (m_tokenizer.peek() == "")
		m_readStatus = HEADER_FIELDS_END;
}

// bool
// HttpRequestParser::checkHeaderFields(HeaderFieldsMap& headerFieldsMap)
// {
//     bool	check = true;
//
//     check &= headerFieldsMap.count("HOST") > 0;
//
//     if (check == false)
//         throw HttpErrorHandler(400);
//     return check;
// }
