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
	method += ' ';
	// TODO
	// add all method
	if (method == "GET ")
		request.m_method = RequestHandler::GET;
	else if (method == "HEAD ")
		request.m_method = RequestHandler::HEAD;
	else if (method == "POST ")
		request.m_method = RequestHandler::POST;
	else if (method == "PUT ")
		request.m_method = RequestHandler::PUT;
	else if (method == "DELETE ")
		request.m_method = RequestHandler::DELETE;
	// TODO
	// else if (method == "TRACE ")
		// request.m_method = RequestHandler::TRACE;
	else
		// TODO
		// reqeust.m_status = ??;
		request.m_method = RequestHandler::ERROR;
	m_readStatus = REQUEST_LINE;
}

void
HttpRequestParser::parseStatusLine(Request &request)
{
	const string	statusLine = m_tokenizer.getline();
	string			method;
	size_t			spacePos;
	size_t			queryStringPos;

	spacePos = statusLine.find(" ");
	if (spacePos == string::npos)
	{
		// TODO
		// reqeust.m_status = ??;
		// throw HttpErrorHandler(400);
	}
	request.m_uri = statusLine.substr(0, spacePos);
	queryStringPos = request.m_uri.find("?");
	if (queryStringPos != std::string::npos)
	{
		request.m_queryString.assign(request.m_uri, queryStringPos + 1, std::string::npos);
		request.m_uri = request.m_uri.substr(0, queryStringPos);
	}
	request.m_protocol = statusLine.substr(spacePos + 1);
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

	if (headerLine.size() != 0 && headerLine[0] == ' ')
		return ;
	curPos = 0;
	pos = headerLine.find(": ");
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
			headerFieldsMap[field].push_back(Util::toLower(value));
			continue;
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
