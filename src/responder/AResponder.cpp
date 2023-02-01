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
	m_methodStatus(HEADER)
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
AResponder::writeFile()
{
	ofstream file;
	string	filePath = m_request.m_path + m_request.m_file;

	file.open(filePath.c_str());
	cout << filePath << endl;
	if (file.fail())
		throw runtime_error("file open error");
	file << m_recvBuffer;
	file.close();
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

int
AResponder::chunkedReadBody()
{
	return (0);
}

int
AResponder::normalReadBody()
{
	size_t contentLen;

	contentLen = Util::toInt(m_request.m_headerFieldsMap["CONTENT-LENGTH"][0]);
	cout << "put recv buffer :" << m_recvBuffer << endl;
	cout << contentLen << endl;

	if (m_recvBuffer.size() == contentLen)
	{
		writeFile();
		return (1);
	}
	return (0);
}
