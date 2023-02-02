#include <stdexcept>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include "VirtualServer.hpp"
#include "io/Buffer.hpp"
#include "util/Util.hpp"
#include "AResponder.hpp"

using namespace std;

#define FILE_BUFFER_SIZE (8192)

// constructors & destructor
AResponder::AResponder(RequestHandler& requestHandler)
:	m_requestHandler(requestHandler),
	m_request(m_requestHandler.m_request),
	m_sendBuffer(m_requestHandler.m_sendBuffer),
	m_recvBuffer(m_requestHandler.m_recvBuffer),
	m_methodStatus(HEADER),
	m_chunkedSize(-1)
{
}

AResponder::~AResponder()
{
}


// operators
AResponder&
AResponder::operator=(const AResponder& aMethod)
{
	(void)aMethod;
	return *this;
}

// member functions
void
AResponder::readFile(std::string& readBody)
{
	struct stat		fileStatus;
	std::fstream	file;
	std::string		fileBuffer;
	std::string 	filePath = m_request.m_path + m_request.m_file;

	if (m_request.m_status != 200)
	{
		map<int, string>*	error_page;
		std::string			root;
		// filePath = "/Users/soum/webserv/html/error.html";
		// TODO: expand this function and change its name or handle this behavior out of readFile() function.
		if (m_request.m_locationBlock == NULL)
		{
			error_page = &m_request.m_virtualServer->m_errorPageTable;
			root = m_request.m_virtualServer->m_root;
		}
		else
		{
			error_page = &m_request.m_locationBlock->m_errorPageTable;
			root = m_request.m_locationBlock->m_root;
		}
		if(error_page->count(m_request.m_status) != 0)
		{
			filePath = root + (*error_page)[m_request.m_status];
			// if filePath.find('*') >= size(), exception is thrown.
			// filePath.replace(filePath.find('*'), 1, Util::toString(m_request.m_status));
		}
		else
		{
			readBody = RequestHandler::makeErrorPage(m_request.m_status);
			return;
		}
	}
	file.open(filePath.c_str());
	stat(filePath.c_str(), &fileStatus);
	readBody.clear();
	readBody.reserve(fileStatus.st_size);
	while (!file.eof())
	{
		fileBuffer.resize(FILE_BUFFER_SIZE);
		file.read(&fileBuffer[0], FILE_BUFFER_SIZE);
		fileBuffer.resize(file.gcount());
		readBody += fileBuffer + "\n";
	}
	file.close();
}

void
AResponder::openFile()
{
	string	filePath = m_request.m_path + m_request.m_file;

	// TODO: check permission
	m_file.open(filePath.c_str() , ios::out | ios::trunc);
	if (m_file.fail())
		throw runtime_error("file open error");
}

void
AResponder::writeFile(int writeSize)
{
	m_file.write(m_recvBuffer.data(), writeSize);
}

bool
AResponder::checkFileExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFREG))
		return (true);
	return (false);
}

bool
AResponder::checkDirExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFDIR))
		return (true);
	return (false);

}

void
AResponder::endResponse()
{
	m_requestHandler.m_parser.m_readStatus = HttpRequestParser::FINISHED;
}

void
AResponder::respondHeader()
{
	m_sendBuffer.append("Content-Type: " + m_requestHandler.findContentType(m_request.m_file));
	m_sendBuffer.append(g_CRLF);
	if (m_request.m_status >= 300 ||
			m_request.m_headerFieldsMap.count("CONNECTION") == 0)
		m_sendBuffer.append("Connection: close");
	else
		m_sendBuffer.append("Connection: " + m_request.m_headerFieldsMap["CONNECTION"][0]);
	m_sendBuffer.append(g_CRLF);
}

void
AResponder::readRequestBody()
{
	int status = 0;

	if (m_request.m_headerFieldsMap.count("CONTENT-LENGTH") > 0)
		status = normalReadBody();
	else
		status = chunkedReadBody();
	if (status)
	{
		string readBody;

		m_request.m_status = 201;
		readFile(readBody);
		m_sendBuffer.append("Content-Length: ");
		m_sendBuffer.append(Util::toString(readBody.size()));
		m_sendBuffer.append(g_CRLF);
		m_sendBuffer.append(g_CRLF);
		m_sendBuffer.reserve(m_sendBuffer.size() + readBody.size());
		m_sendBuffer.append(readBody);
		m_methodStatus = DONE;
	}
}

string
AResponder::parseChunkSize()
{
	string hex;
	size_t crlfPos = m_recvBuffer.find(g_CRLF);

	// TODO: update buffer
	if (crlfPos == string::npos)
		return ("");
	hex = m_recvBuffer.substr(0, crlfPos);
	m_recvBuffer.erase(0, crlfPos + 2);
	return (hex);
}

int
AResponder::chunkedReadBody()
{
	while(m_recvBuffer.size() != 0)
	{
		if (m_chunkedSize == -1)
		{
			cout << "before buffer size : " << m_recvBuffer.size() << endl;
			cout << "before Buffer : " << m_recvBuffer << "$" << endl;
			m_chunkedSize = Util::hexToDecimal(parseChunkSize());
			cout << "chunked size : "<< m_chunkedSize << endl;
			cout << "buffer size : " << m_recvBuffer.size() << endl;
			cout << "after Buffer : " << m_recvBuffer << "$" << endl;
		}
		if (m_chunkedSize == 0)
		{
			cout << "buffer size : " << m_recvBuffer.size() << endl;
			return (1);
		}
		if (m_chunkedSize != -1 && m_chunkedSize + 2 <= (int)m_recvBuffer.size())
		{
			writeFile(m_chunkedSize);
			m_recvBuffer.erase(0, m_chunkedSize + 2);
			m_chunkedSize = -1;
		}
		else if (m_chunkedSize != -1 && m_chunkedSize > (int)m_recvBuffer.size())
		{
			writeFile(m_recvBuffer.size());
			m_chunkedSize -= m_recvBuffer.size();
			m_recvBuffer.clear();
		}
		else
			return (0);
	}
	return (0);
}

int
AResponder::normalReadBody()
{
	size_t contentSize;

	contentSize = Util::toInt(m_request.m_headerFieldsMap["CONTENT-LENGTH"][0]);
	if (m_recvBuffer.size() == contentSize)
	{
		writeFile(contentSize);
		return (1);
	}
	return (0);
}

