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
				parseStatusLine(request);
				break;
			case HEADER_FIELDS:
				parseHeaderFields(request.m_headerFieldsMap);
				break;
			case HEADER_FIELDS_END:
				checkHeaderFields(request.m_headerFieldsMap);
				if (request.m_method == RequestHandler::GET) // if (isReadMethod(request.m_method) == true)
					break;
				m_tokenizer.updateBufferForBody();
				// fall through
			case MESSAGE_BODY:
				// normal transfer or chunked
				parseMessageBody(request);
				break;
			case FINISHED:
				// trailer section is not implemented
				// throw HttpErrorHandler(501);
				break;
			default:
				throw std::logic_error("unhandled read status in\
HttpRequestParser::parse()");
		}
	}
	m_tokenizer.flush();
//	Logger::log(Logger::INFO, *request.m_httpInfo);
}

/*
void
HttpRequestParser::parse(Request& request)
{
	// using function pointer table instead of switch statement?
	string::size_type	pos;

	pos = updateBuffer();
	LOG(DEBUG, "buffer status = %d", pos);
	if (pos == string::npos)
		return;
	while (m_tokenizer.empty() == false)
	{
		(void)request;
		cout << m_tokenizer.get() << '\n';
		switch (m_readStatus)
		{
			case REQUEST_LINE:
				parseStatusLine(request);
				break;
			case HEADER_FIELDS:
				parseHeaderFields(request.m_httpInfo->m_requestHeaderFields);
				break;
			case MESSAGE_BODY:
				// normal transfer or chunked
				parseMessageBody();
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
		*/

string::size_type
HttpRequestParser::updateBuffer()
{
	if (m_readStatus < MESSAGE_BODY)
		return m_tokenizer.updateBufferForHeader();
	else
		return m_tokenizer.updateBufferForBody();
}

void
HttpRequestParser::parseStatusLine(Request &request)
{
	const string	statusLine = m_tokenizer.getline();
	string			method;

	method = Util::toUpper(statusLine.substr(0, statusLine.find(" ")));
	if (method == "GET")
		request.m_method = RequestHandler::GET;
	else if (method == "HEAD")
		request.m_method = RequestHandler::HEAD;
	else if (method == "POST")
		request.m_method = RequestHandler::POST;
	else if (method == "PUT")
		request.m_method = RequestHandler::PUT;
	else if (method == "DELETE")
		request.m_method = RequestHandler::DELETE;
	else
		throw HttpErrorHandler(405);
	request.m_uri = statusLine.substr(statusLine.find(" ") + 1,
			statusLine.rfind(" ") - statusLine.find(" "));
	request.m_protocol = statusLine.substr(statusLine.rfind(" ") + 1);
	m_readStatus = checkStatusLine(request);
}

/* TODO
 * find location block from uri
 */
HttpRequestParser::e_readStatus
HttpRequestParser::checkStatusLine(Request &request)
{
//	findLocationBlock(request);
	if (request.m_protocol != "HTTP/1.1")
		throw HttpErrorHandler(505);
	return (HEADER_FIELDS);
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
			headerFieldsMap[field].push_back(value);
			break;
		}
		pos++;
		value = headerLine.substr(curPos, pos - curPos);
		curPos = headerLine[pos] == ' ' ? pos + 1 : pos;
		if (value[value.length() - 1] == ',')
			value.erase(value.end() - 1);
		headerFieldsMap[field].push_back(value);
	}
	if (m_tokenizer.peek() == "\r\n")
		m_readStatus = HEADER_FIELDS_END;
}

bool
HttpRequestParser::checkHeaderFields(HeaderFieldsMap& headerFieldsMap)
{
	(void)headerFieldsMap;
	return true;
}

void
HttpRequestParser::parseMessageBody(Request& request)
{
	(void)request;
	cout << "%% body %%" << m_tokenizer.get() << "%% body end %%";
}

HttpRequestParser::e_readStatus
HttpRequestParser::getReadStatus() const
{
	return m_readStatus;
}
