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
	getline(ss, requestLine, '\r');
	if (ss.fail())
		return (retVal) ;
	if (requestLine == "\n")
	{
		m_requestSection = REQUEST_BODY;
		return (1);
	}
	stringstream requestLineStream(requestLine);
	while (requestLineStream >> token)
	{
		if (requestLineStream.fail())
			return (retVal);
		retVal += token.size() + 1;
		if (m_methodType == "")
			m_methodType = token;
		else if (m_uri == "")
			m_uri = token;
		else
		{
			if (token != "HTTP/1.1")
			{
				// throw exception;
			}
		};
	}
	return (retVal);
}

int
Request::makeRequestHeader(const string& buffer, requestHeaderMap& requestHeaderMap)
{
	stringstream	ss(buffer);
	string			requestLine;
	string			token;
	int				retVal;

	retVal = 0;
	while (getline(ss, requestLine, '\r'))
	{
		if (ss.fail())
			return (retVal);
		if (requestLine == "\n")
		{
			m_requestSection = REQUEST_BODY;
			return (1);
		}
	}
	return (retVal);
}

void
Request::makeReqeustBody(const string& buffer)
{
	(void)buffer;
}
