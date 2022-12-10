#include "Request.hpp"

Request::Request()
{
	m_requestSection = REQUEST_LINE;
	m_methodType = "";
	m_uri = "";
}

Request::~Request()
{
}

int
Request::getRequestSection() const
{
	return (m_requestSection);
}

int
Request::makeRequestLine(const string& buffer)
{
	stringstream	ss(buffer);
	string			requestLine;
	string			token;
	int				retVal;

	retVal = 0;
	while (getline(ss, requestLine, '\n'))
	{
		if (requestLine[requestLine.size() - 1] != '\r')
			return (retVal) ;
		if (requestLine == "\r")
		{
			m_requestSection = REQUEST_HEADER;
			return (retVal + 2);
		}
		stringstream requestLineStream(requestLine);
		retVal = requestLine.size() + 1;
		while (requestLineStream >> token)
		{
			if (m_methodType == "")
				m_methodType = token;
			else if (m_uri == "")
			{
				m_uri = token;
				// checkUri();
			}
			else
			{
				if (token != "HTTP/1.1")
				{
					// throw exception;
				}
			};
		}
	}
	return (retVal);
}

int
Request::makeRequestHeader(const string& buffer, requestHeaderMap& requestHeaderMap)
{
	stringstream	ss(buffer);
	string			requestLine;
	string			requestHeaderField;
	string			token;
	int				retVal;

	retVal = 0;
	while (getline(ss, requestLine, '\r'))
	{
		if (requestLine[requestLine.size() - 1] != '\r')
			return (retVal) ;
		if (requestLine == "\r")
		{
			m_requestSection = REQUEST_BODY;
			return (retVal + 2);
		}
		stringstream requestLineStream(requestLine);
		retVal = requestLine.size() + 1;
		requestLineStream >> requestHeaderField;
		requestHeaderField.pop_back();
		while (requestLineStream >> token)
		{
			if (token[token.size() - 1] == ',')
				token.pop_back();
			requestHeaderMap[requestHeaderField].push_back(token);
		}
	}
	return (retVal);
}

void
Request::makeReqeustBody(const string& buffer)
{
	(void)buffer;
}
