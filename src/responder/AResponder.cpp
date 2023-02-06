#include <sys/stat.h>
#include <fcntl.h>

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Logger.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "VirtualServer.hpp"
#include "http/RequestHandler.hpp"
#include "io/Buffer.hpp"
#include "util/Util.hpp"
#include "AResponder.hpp"

#ifndef TEMP_DIR
# define TEMP_DIR "/tmp"
#endif

#define FILE_BUFFER_SIZE (8192)

extern const std::string	g_tempDir = TEMP_DIR;

using namespace std;

// constructors & destructor
AResponder::AResponder(RequestHandler& requestHandler)
:	m_requestHandler(requestHandler),
	m_request(m_requestHandler.m_request),
	m_sendBuffer(m_requestHandler.m_sendBuffer),
	m_recvBuffer(m_requestHandler.m_recvBuffer),
	m_responseStatus(RES_HEADER),
	m_dataSize(-1)
{
	// TODO: condition is not complete.
	if (m_request.m_headerFieldsMap.count("Transfer-Encoding") == 1
		&& m_request.m_headerFieldsMap["Transfer-Encoding"].back() == "chunked")
		m_recvContentFunc = &AResponder::receiveContentChunked;
	else
		m_recvContentFunc = &AResponder::receiveContentNormal;
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
string
AResponder::getErrorPage(string& readBody)
{
	map<int, string>*	error_page;
	string				root;
	string				filePath = "";

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
	}
	else
	{
		readBody = RequestHandler::makeErrorPage(m_request.m_status);
	}
	return (filePath);
}

// member functions
void
AResponder::readFile(std::string& readBody)
{
	struct stat		fileStatus;
	std::fstream	file;
	std::string		fileBuffer;
	std::string 	filePath = m_request.m_path + m_request.m_file;

	if (readBody.empty() == false)
		return;
	file.open(filePath.c_str());
	stat(filePath.c_str(), &fileStatus);
	readBody.clear();
	readBody.reserve(fileStatus.st_size);
	while (!file.eof())
	{
		fileBuffer.resize(FILE_BUFFER_SIZE);
		file.read(&fileBuffer[0], FILE_BUFFER_SIZE);
		fileBuffer.resize(file.gcount());
		readBody += fileBuffer;
	}
	file.close();
}

void
AResponder::openFile()
{
	openFile(m_request.m_path + m_request.m_file);
}

void
AResponder::openFile(const string& path)
{
	// TODO: check permission
	m_fileFd = open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH);
	if (m_fileFd < 0)
		throw runtime_error("AResponder::openFile() open error");
}

void
AResponder::writeToFile(int writeSize)
{
	// TODO
	// m_fileFd가 pipe[1] (cgi)
	write(m_fileFd, m_recvBuffer.data(), writeSize);
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
	if (m_request.m_status >= 300)
		m_requestHandler.m_parser.m_readStatus = HttpRequestParser::ERROR;
}

void
AResponder::respondHeader()
{
	m_sendBuffer.append("Server: webserv/2.0");
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append("Date: " + Util::getDate("%a, %d %b %Y %X %Z"));
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append("Content-Type: " + m_requestHandler.findContentType(m_request.m_file));
	m_sendBuffer.append(g_CRLF);
	if (m_request.m_status == 405)
	{
		if (m_request.m_locationBlock != NULL)
			m_sendBuffer.append("Allow:" + RequestHandler::methodToString(m_request.m_locationBlock->m_limitExcept));
		else
			m_sendBuffer.append("Allow:" + RequestHandler::methodToString(0x1f));

		m_sendBuffer.append(g_CRLF);
	}
	// INFO: is reconnecting right when responding with redirection?
	if (m_request.m_status >= 300)
	{
		m_sendBuffer.append("Connection: close");
		m_sendBuffer.append(g_CRLF);
	}
	else if (m_request.m_headerFieldsMap.count("CONNECTION") > 0)
	{
		m_sendBuffer.append("Connection: " + m_request.m_headerFieldsMap["CONNECTION"][0]);
		m_sendBuffer.append(g_CRLF);
	}
	if (m_request.m_status == 201)
	{
		m_sendBuffer.append("Location: " + m_request.m_path + m_request.m_file);
		m_sendBuffer.append(g_CRLF);
	}
}

void
AResponder::respondBody(const string& readBody)
{
	m_sendBuffer.append("Content-Length: ");
	m_sendBuffer.append(Util::toString(readBody.size()));
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(readBody);
}

int
AResponder::sendContentNormal()
{
	return 0;
}

int
AResponder::sendContentChunked()
{
	return 0;
}

string
AResponder::parseChunkSize()
{
	string hex;
	size_t crlfPos = m_recvBuffer.find(g_CRLF);

	// TODO: update buffer
	if (crlfPos == string::npos)
		return ("");
	if (crlfPos == 0)
	{
		m_recvBuffer.erase(0, 2);
		return (parseChunkSize());
	}
	hex = m_recvBuffer.substr(0, crlfPos);
	m_recvBuffer.erase(0, crlfPos + 2);
	return (hex);
}

int
AResponder::receiveContentChunked()
{
	while(m_recvBuffer.size() != 0)
	{
		if (m_dataSize == -1)
			m_dataSize = Util::hexToDecimal(parseChunkSize());
		// INFO: clientMaxBodySize may be total content length?
		if (m_dataSize > m_request.m_locationBlock->m_clientMaxBodySize)
			throw (413);
		if (m_dataSize == 0 && m_recvBuffer.size() == 2)
		{
			m_recvBuffer.clear();
			return (1);
		}
		if (m_dataSize > 0 && m_dataSize + 2 <= (int)m_recvBuffer.size())
		{
			writeToFile(m_dataSize);
			m_recvBuffer.erase(0, m_dataSize + 2);
			m_dataSize = -1;
		}
		else if (m_dataSize > 0 && m_dataSize >= (int)m_recvBuffer.size())
		{
			writeToFile(m_recvBuffer.size());
			m_dataSize -= m_recvBuffer.size();
			m_recvBuffer.clear();
			if (m_dataSize == 0)
				m_dataSize = -1;
		}
		else
			return (0);
	}
	return (0);
}

int
AResponder::receiveContentNormal()
{
	if (m_dataSize == -1)
		m_dataSize = Util::toInt(m_request.m_headerFieldsMap["CONTENT-LENGTH"][0]);
	if (m_dataSize == 0)
		return 1;

	int	count = m_recvBuffer.send(m_fileFd);
	m_dataSize -= count;
	if (m_dataSize == 0)
		return 1;
	return (0);
}

void
AResponder::respondStatusLine(int statusCode)
{
	m_sendBuffer.append(g_httpVersion);
	m_sendBuffer.append(" ");
	m_sendBuffer.append(Util::toString(statusCode));
	m_sendBuffer.append(" ");
	m_sendBuffer.append(HttpErrorHandler::getErrorMessage(statusCode));
	m_sendBuffer.append(g_CRLF);
}
