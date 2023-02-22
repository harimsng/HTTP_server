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
HttpRequestParser::parse(Request& request) try
{
	string::size_type	pos;

	pos = m_tokenizer.updateBuffer();
	while (pos != string::npos)
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
				;
		}
		if (m_readStatus == HEADER_FIELDS_END)
			return;
		pos = m_tokenizer.updateBuffer();
	}
}
catch (int statusCode)
{
	UPDATE_REQUEST_ERROR(request.m_status, statusCode);
	m_readStatus = HEADER_FIELDS_END;
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
	if (RequestHandler::s_methodConvertTable.count(method) != 0)
		request.m_method = RequestHandler::s_methodConvertTable[method];
	else
		throw 400;
	m_readStatus = REQUEST_LINE;
	m_tokenizer.flush();
}

void
HttpRequestParser::parseStatusLine(Request &request)
{
	const string	statusLine = m_tokenizer.getline();
	string			method;
	size_t			spacePos;
	size_t			queryStringPos;

	m_readStatus = HEADER_FIELDS;
	spacePos = statusLine.find(" ");
	if (spacePos == string::npos)
	{
		throw 400;
	}
	request.m_uri = statusLine.substr(0, spacePos);
	queryStringPos = request.m_uri.find("?");
	if (queryStringPos != std::string::npos)
	{
		request.m_queryString.assign(request.m_uri, queryStringPos + 1, std::string::npos);
		request.m_uri = request.m_uri.substr(0, queryStringPos);
	}
	request.m_protocol = statusLine.substr(spacePos + 1);
	m_tokenizer.flush();
}

#include "Logger.hpp"

void
HttpRequestParser::parseHeaderFields(HeaderFieldsMap& headerFieldsMap)
{
	const string headerLine = m_tokenizer.getline();
	string	field;
	string	value;
	size_t	pos;
	size_t	curPos;

	LOG(DEBUG, "%s", headerLine.c_str());
	if (headerLine == g_CRLF)
	{
		m_readStatus = HEADER_FIELDS_END;
		m_tokenizer.flush();
		return;
	}
	if (headerLine.size() != 0 && headerLine[0] == ' ')
		return ;
	curPos = 0;
	pos = headerLine.find(": ");
	if (pos == string::npos)
	{
		throw 400;
	}
	field = Util::toUpper(headerLine.substr(curPos, pos));
	curPos = pos + 2;
	while (pos != string::npos)
	{
		pos = headerLine.find(") ", curPos);
		if (pos == string::npos)
			pos = headerLine.find(",", curPos);
		if (pos == string::npos)
		{
			value = headerLine.substr(curPos);
			headerFieldsMap[field].push_back(value);
			continue;
		}
		pos++;
		value = headerLine.substr(curPos, pos - curPos);
		curPos = headerLine[pos] == ' ' ? pos + 1 : pos;
		if (value[value.length() - 1] == ',')
			value.erase(value.end() - 1);
		headerFieldsMap[field].push_back(value);
	}
	m_tokenizer.flush();
}
