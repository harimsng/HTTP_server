/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: soum <soum@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 16:26:53 by soum              #+#    #+#             */
/*   Updated: 2022/12/15 19:57:14 by hseong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "communicator/Request.hpp"

#include <iostream>
using namespace std;

Request::Request()
{
	m_requestSection = REQUEST_LINE;
	m_preBufferSize = 0;
}

Request::~Request()
{
}

int
Request::makeRequestLine(const string& buffer)
{
	stringstream	ss(buffer);
	string			requestLine;
	string			token;
	int				retVal;

	retVal = 0;
	getline(ss, requestLine, '\n');
	if (requestLine[requestLine.size() - 1] != '\r')
		return (retVal) ;
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
	m_requestSection = REQUEST_HEADER;
	return (retVal);
}

int
Request::makeRequestHeader(const string& buffer)
{
	stringstream	ss(buffer);
	string			requestLine;
	string			requestHeaderField;
	string			token;
	int				retVal;

	retVal = 0;
	while (getline(ss, requestLine, '\n'))
	{
		if (requestLine[0] == '\r')
		{
			printRequestMessage();
			m_requestSection = REQUEST_HEADER_END;
			return (retVal + 2);
		}
		if (requestLine[requestLine.size() - 1] != '\r')
			return (retVal) ;
		stringstream requestLineStream(requestLine);
		retVal += requestLine.size() + 1;
		requestLineStream >> requestHeaderField;
		requestHeaderField.pop_back();
		while (requestLineStream >> token)
		{
			if (token[token.size() - 1] == ',')
				token.pop_back();
			m_requestHeaderMap[requestHeaderField].push_back(token);
		}
	}
	return (retVal);
}

void
Request::makeReqeustBody(const string& buffer)
{
	(void)buffer;
}

void
Request::printRequestMessage() const
{
	requestHeaderMap::const_iterator mapIt;
	mapIt = m_requestHeaderMap.begin();
	for (; mapIt != m_requestHeaderMap.end(); mapIt++)
	{
		cout << "field :" << mapIt->first << endl;
		for (size_t i = 0; i < mapIt->second.size(); i++)
			cout << "\t" << mapIt->second[i] << " ";
		cout << endl;
	}
}

void
Request::makeRequest(std::string& buffer)
{
	if (m_requestSection == REQUEST_LINE)
		buffer.erase(0, makeRequestLine(buffer));
	if (m_requestSection == REQUEST_HEADER)
		buffer.erase(0, makeRequestHeader(buffer));
	if (m_requestSection == REQUEST_HEADER_END)
		return;

	if (m_requestSection == REQUEST_BODY)
		makeReqeustBody(buffer);
	m_preBufferSize = buffer.size();
}

int&
Request::getRequestSection()
{
	return (m_requestSection);
}
