#include <fcntl.h>
#include <iostream>

#include "ServerManager.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"
#include "GetMethod.hpp"

using namespace std;

// constructors & destructor
GetMethod::GetMethod(RequestHandler& requestHandler)
:	AMethod(requestHandler)
{
}

GetMethod::~GetMethod()
{
}

// operators
GetMethod&
GetMethod::operator=(const GetMethod& getMethod)
{
	(void)getMethod;
	return *this;
}

#ifndef FAST_MODE
void
GetMethod::completeResponse()
{
	fstream file;
	string filePath = m_request.m_path + m_request.m_file;
	string readBody;
	string readLine;

	file.open(filePath);
	while (!file.eof())
	{
		getline(file, readLine);
		if (readLine == "")
			continue;
		readBody += readLine + "\n";
	}
	readBody.pop_back();
	file.close();
	m_sendBuffer.append("Content-Length: ");
	m_sendBuffer.append(Util::toString(readBody.size()));
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(readBody);

	// method must know end of response(content length, chunked)
	endResponse();
}
#else
void
GetMethod::completeResponse()
{
	string filePath = m_request.m_path + m_request.m_file;
	const int	bufferSize = 4096;
	Buffer		buffer;
	int			fd = open(filePath.data(), O_RDONLY);
	string::size_type	readSize;

	// m_sendBuffer.append("
	do
	{
		readSize = buffer.read(fd);
		buffer.write(fd);
	}
	while (readSize == bufferSize);
	close(fd);

	/*
	file.open(filePath);
	while (!file.eof())
	{
		getline(file, readLine);
		if (readLine == "")
			continue;
		readBody += readLine + "\n";
	}
	readBody.pop_back();
	file.close();
	*/
	m_sendBuffer.append("Content-Length: ");
	m_sendBuffer.append(Util::toString(readBody.size()));
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(readBody);
	// 1. m_path == "" , m_file == ""
	// 	-> 404
	// 2. m_path != "", m_file == ""
	// 	-> autoIndex on -> dir listing
	// 	-> autoIndex off -> 405
	// 3. m_path != "", m_file != ""
	//	-> open()
	// reqeust의path
}
#endif
