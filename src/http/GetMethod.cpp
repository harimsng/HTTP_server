#include "GetMethod.hpp"
#include "http/AMethod.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"

// constructors & destructor
#include <iostream>

using namespace std;

GetMethod::GetMethod(Request& request, SendBuffer& sendBuffer, ReceiveBuffer& recvBuffer)
: AMethod(request, sendBuffer, recvBuffer)
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
	// 1. m_path == "" , m_file == ""
	// 	-> 404
	// 2. m_path != "", m_file == ""
	// 	-> autoIndex on -> dir listing
	// 	-> autoIndex off -> 405
	// 3. m_path != "", m_file != ""
	//	-> open()
	// reqeust의path
}
