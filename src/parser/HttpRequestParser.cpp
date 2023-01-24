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
				parseMethod(request);
				break;
			case REQUEST_LINE:
				pos = m_tokenizer.updateBuffer();
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
	else
		request.m_method = RequestHandler::ERROR;
	m_readStatus = REQUEST_LINE;
}

void
HttpRequestParser::parseStatusLine(Request &request)
{
	const string	statusLine = m_tokenizer.getline();
	string			method;

	request.m_uri = statusLine.substr(0, statusLine.find(" "));
	request.m_protocol = statusLine.substr(statusLine.find(" ") + 1);
	m_readStatus = HEADER_FIELDS;
}

// HttpRequestParser::e_readStatus
// HttpRequestParser::checkStatusLine(Request &request)
// {
//     //
//     if (request.m_protocol != g_httpVersion)
//         throw HttpErrorHandler(505);
//     return (HEADER_FIELDS);
// }

void
HttpRequestParser::parseHeaderFields(HeaderFieldsMap& headerFieldsMap)
{
	const string headerLine = m_tokenizer.getline();
	string	field;
	string	value;
	size_t	pos;
	size_t	curPos;

	cout << headerLine << endl;
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
			headerFieldsMap[field].push_back(Util::toUpper(value));
			break;
		}
		pos++;
		value = headerLine.substr(curPos, pos - curPos);
		curPos = headerLine[pos] == ' ' ? pos + 1 : pos;
		if (value[value.length() - 1] == ',')
			value.erase(value.end() - 1);
		headerFieldsMap[field].push_back(Util::toUpper(value));
	}
	if (m_tokenizer.peek() == "\r\n")
		// checkHeaderFields(headerFieldsMap);
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
