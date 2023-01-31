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
		// filePath = "/Users/soum/webserv/html/error.html";
		// TODO: expand this function and change its name or handle this behavior out of readFile() function.
		if (m_request.m_locationBlock == NULL)
		{
			map<int, string>&	error_page = m_request.m_virtualServer->m_errorPageTable;
			std::string			filePath = m_request.m_virtualServer->m_root + error_page[m_request.m_status];
		}
		else
		{
			map<int, string>&	error_page = m_request.m_locationBlock->m_errorPageTable;
			std::string			filePath = m_request.m_locationBlock->m_root + error_page[m_request.m_status];
		}
		map<int, string>& error_page = m_request.m_locationBlock->m_errorPageTable;
		if(error_page.size() != 0 && error_page.count(m_request.m_status) != 0)
		{
			filePath = m_request.m_locationBlock->m_root + error_page[m_request.m_status];
			filePath.replace(filePath.find('*'), 1, Util::toString(m_request.m_status));
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
