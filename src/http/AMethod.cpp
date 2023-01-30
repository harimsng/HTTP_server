#include "Webserv.hpp"
#include "AMethod.hpp"
#include "VirtualServer.hpp"

#include <algorithm>
#include <iostream>

using namespace std;

// constructors & destructor
AMethod::AMethod(RequestHandler& requestHandler)
:	m_requestHandler(requestHandler),
	m_request(m_requestHandler.m_request),
	m_sendBuffer(m_requestHandler.m_sendBuffer),
	m_recvBuffer(m_requestHandler.m_recvBuffer),
	m_methodStatus(HEADER)
{
}

AMethod::~AMethod()
{
}


// operators
AMethod&
AMethod::operator=(const AMethod& aMethod)
{
	(void)aMethod;
	return *this;
}

// member functions
void
AMethod::completeResponse()
{

}

void
AMethod::readFile(std::string& readBody)
{
	struct stat		fileStatus;
	std::fstream	file;
	std::string		readLine;
	std::string 	filePath = m_request.m_path + m_request.m_file;

	if (m_request.m_status != 200)
	{
		vector<string> error_page;

		filePath = "/Users/soum/webserv/html/error.html";
		if (m_request.m_locationBlock == NULL)
		{
			error_page = m_request.m_virtualServer->m_errorPage;
			if(error_page.size() != 0 && (find(error_page.begin(), error_page.end() - 1, Util::toString(m_request.m_status))
				!= error_page.end() - 1))
				filePath = m_request.m_virtualServer->m_root + error_page.back();
		}
		else
		{
			error_page = m_request.m_locationBlock->m_errorPage;
			if(error_page.size() != 0 && (find(error_page.begin(), error_page.end() - 1, Util::toString(m_request.m_status))
				!= error_page.end() - 1))
				filePath = m_request.m_locationBlock->m_root + error_page.back();
		}
	}
	file.open(filePath.c_str());
	stat(filePath.c_str(), &fileStatus);
	readBody.clear();
	readBody.reserve(fileStatus.st_size);
	// INFO: it doesn't send raw content of resource.
	while (!file.eof())
	{
		std::getline(file, readLine);
		if (readLine == "")
			continue;
		readBody += readLine + "\n";
	}
	readBody.erase(readBody.end() - 1);
	file.close();
}


bool
AMethod::checkFileExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFREG))
		return (true);
	return (false);
}

bool
AMethod::checkDirExists(const std::string& filePath)
{
	struct stat buffer;
	int			exist;

	exist = stat(filePath.c_str(), &buffer);
	if (exist == 0 && ((buffer.st_mode & S_IFMT) == S_IFDIR))
		return (true);
	return (false);

}

void
AMethod::endResponse()
{
	m_requestHandler.m_parser.m_readStatus = HttpRequestParser::FINISHED;
}

void
AMethod::completeResponseHeader()
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
