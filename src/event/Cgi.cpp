#include <sys/wait.h>

#include <functional>
#include <stdexcept>
#include <unistd.h>
#include <iostream>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "VirtualServer.hpp"
#include "http/RequestHandler.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "Cgi.hpp"

using namespace std;

std::vector<int>	Cgi::s_cgiPipeList;

extern const string	g_webservDir;

// deleted
Cgi&	Cgi::operator=(Cgi const& cgi)
{
	(void)cgi;

	return *this;
}

Cgi::Cgi(Cgi const& cgi)
:	EventObject()
{
	(void)cgi;
}

// constructors & destructor
Cgi::Cgi(int cgiToServer[2], int serverToCgi[2], RequestHandler& requestHandler, Buffer& toCgiBuffer)
:	EventObject(cgiToServer[0]),
	m_requestHandler(&requestHandler),
	m_toCgiBuffer(&toCgiBuffer),
	m_state(CGI_HEADER)
{
	initialize(requestHandler.getRequest());
	executeCgi(cgiToServer, serverToCgi);
	ServerManager::registerEvent(cgiToServer[0], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, this);
	ServerManager::registerEvent(serverToCgi[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_WRITE, this);
	LOG(DEBUG, "[%d] registering cgi read event", cgiToServer[0]);
	LOG(DEBUG, "[%d] registering cgi write event", serverToCgi[1]);
	m_fd = cgiToServer[0];
	m_serverToCgi = serverToCgi[1];
}

Cgi::Cgi(int cgiToServer[2], RequestHandler& requestHandler)
:	EventObject(cgiToServer[0]),
	m_requestHandler(&requestHandler),
	m_state(CGI_HEADER)
{
	initialize(requestHandler.getRequest());
	executeCgi(cgiToServer);
	ServerManager::registerEvent(cgiToServer[0], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, this);
	LOG(DEBUG, "[%d] registering cgi read event", cgiToServer[0]);
	m_fd = cgiToServer[0];
}

Cgi::~Cgi()
{
	static short unsigned int	count = 0;

	close(m_fd);
	waitpid(m_pid, NULL, 0);
	LOG(INFO, "[%5hu][%5d] cgi exited", count++, m_fd);
}

// event handlers called by event poller
Cgi::IoEventPoller::EventStatus
Cgi::handleReadEventWork()
{
	if (receiveCgiResponse() == 0)
	{
		return IoEventPoller::STAT_END;
	}
	return IoEventPoller::STAT_NORMAL;
}

Cgi::IoEventPoller::EventStatus
Cgi::handleWriteEventWork()
{
	sendCgiRequest();
	return IoEventPoller::STAT_NORMAL;
}

Cgi::IoEventPoller::EventStatus
Cgi::handleErrorEventWork()
{
	if (m_eventStatus == EVENT_EOF)
		return IoEventPoller::STAT_END;
	return IoEventPoller::STAT_ERROR; // STAT_ERROR is currently ignored.
}

// member functions
int
Cgi::receiveCgiResponse()
{
	int cnt;
	int statusCode;

	cnt = m_fromCgiBuffer.receive(m_fd);
	LOG(DEBUG, "[%d] receiveCgiResponse() count = %d", m_fd, cnt);
	if (cnt == -1 || (cnt == 0 && m_eventStatus != EventObject::EVENT_EOF))
		return -1;
	switch (m_state)
	{
		case Cgi::CGI_HEADER:
			statusCode = parseCgiHeader();
			if (m_state != CGI_CONTENT)
				break;
			respondStatusLine(statusCode);
			respondHeader();
			m_requestHandler->m_sendBuffer.append("Transfer-Encoding: chunked");
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			if (m_fromCgiBuffer.size() == 0)
			{
				break;
			}
// fall through
		case Cgi::CGI_CONTENT:
			m_requestHandler->m_sendBuffer.append(Util::toHex(m_fromCgiBuffer.size()));
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			m_requestHandler->m_sendBuffer.append(m_fromCgiBuffer);
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			m_fromCgiBuffer.clear();
			if (cnt == 0)
			{
				m_requestHandler->resetStates();
			}
			break;
	}
	return (cnt);
}

// if output is slower than input, the buffer grows up indefinitely.
int
Cgi::sendCgiRequest()
{
	// NOTE: there's chance for blocking because we don't know pipe memory left.
	int	count = m_toCgiBuffer->send(m_serverToCgi);

//	if (count != 0)
	{
		LOG(DEBUG, "[%d] sendCgiRequest() count = %d", m_serverToCgi, count);
	}
	if (m_toCgiBuffer->size() == 0 && m_toCgiBuffer->status() == Buffer::BUF_EOF)
	{
		LOG(DEBUG, "[%d] sendCgiRequest() end", m_serverToCgi);
		close(m_serverToCgi);
		return -1;
	}
	return count;
}

int
Cgi::parseCgiHeader()
{
	int	start = 0, end;
	int statusCode = 200;
	string	headerField;
	string	fieldName;
	string	fieldValue;

	if (m_fromCgiBuffer.find("\r\n\r\n") == string::npos)
		return statusCode;

	while (1)
	{
		end = m_fromCgiBuffer.find(g_CRLF, start);
		headerField = m_fromCgiBuffer.substr(start, end - start);
		if (headerField.empty())
			break;
		fieldName = headerField.substr(0, headerField.find(':'));
		fieldName = Util::toUpper(fieldName);
		fieldValue = headerField.substr(headerField.find(':') + 1);
		if (fieldName == "STATUS")
			statusCode = Util::toInt(fieldValue);
		else
			m_responseHeader += headerField + g_CRLF;
		start = end + 2;
	}
	m_fromCgiBuffer.erase(0, end + 2);
	m_state = CGI_CONTENT;
	return (statusCode);
}

void
Cgi::initialize(const Request &request)
{
	std::vector<std::string>	envBase;
	std::vector<std::string>	argvBase;
	std::string 				path;
	std::string					scriptPath;
	std::string					queryString;
	std::string					ext;

	// INFO: dangerous
	ext = request.m_file.substr(request.m_file.rfind("."));
	m_cgiPath = request.m_virtualServer->m_cgiPass[ext];
	path = request.m_path + request.m_file;
	// NOTE: CONTENT_LENGTH=-1 is not compliant with RFC 3875. it defines it same as below
	// CONTENT_LENGTH = "" | 1*digit
	// digit = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
    std::string CONTENT_LENGTH = "CONTENT_LENGTH=-1";
    std::string CONTENT_TYPE = "CONTENT_TYPE=";
    std::map<std::string, std::vector<std::string> >::const_iterator contentIt;
    contentIt = request.m_headerFieldsMap.find("CONTENT-TYPE");
    if (contentIt != request.m_headerFieldsMap.end())
    {
        CONTENT_TYPE += contentIt->second[0];
    }
	std::string	HTTP_X_SECRET_HEADER_FOR_TEST;
    contentIt = request.m_headerFieldsMap.find("X-SECRET-HEADER-FOR-TEST");
    if (contentIt != request.m_headerFieldsMap.end())
    {
    	HTTP_X_SECRET_HEADER_FOR_TEST += "HTTP_X_SECRET_HEADER_FOR_TEST=" + contentIt->second[0];
    }
	// NOTE: ...
	/*
    if (request.m_method == RequestHandler::POST && request.m_bodySize > 0)
    {
        CONTENT_LENGTH += Util::toString(request.m_bodySize);
    }
	*/
    std::string SERVER_SOFTWARE = "SERVER_SOFTWARE=webserv/2.0";
    std::string SERVER_PROTOCOL = "SERVER_PROTOCOL=HTTP/1.1"; // different GET POST
    std::string GATEWAY_INTERFACE = "GATEWAY_INTERFACE=CGI/1.1";

    std::string SERVER_PORT = "SERVER_PORT=";
    std::string SERVER_NAME = "SERVER_NAME=";
    std::string REMOTE_ADDR = "REMOTE_ADDR=";
    std::string REMOTE_HOST = "REMOTE_HOST=";

    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200"; // php-cgi direct exec
    std::string REQUEST_METHOD = "REQUEST_METHOD=" + RequestHandler::s_methodRConvertTable[request.m_method]; // 실제 메소드 이름으로 수정 필요
    std::string REQUEST_URI = "REQUEST_URI=" + request.m_uri;
    std::string PATH_INFO = "PATH_INFO=" + request.m_uri;
    std::string PATH_TRANSLATED = "PATH_TRANSLATED=" + request.m_path + request.m_file;
    std::string SCRIPT_NAME = "SCRIPT_NAME=" + request.m_file;
    std::string SCRIPT_FILENAME = "SCRIPT_FILENAME=" + request.m_path + request.m_file; // supposed to be path of cgi script in file-system
    std::string QUERY_STRING = "QUERY_STRING=";
    if (request.m_method == RequestHandler::GET)
    {
        QUERY_STRING += request.m_queryString;
	}
	envBase.reserve(32);
	m_envp.reserve(32);
	m_argv.reserve(32);
    envBase.push_back(REDIRECT_STATUS);
    envBase.push_back(CONTENT_LENGTH);
    envBase.push_back(CONTENT_TYPE);
    envBase.push_back(SERVER_PROTOCOL);
    envBase.push_back(GATEWAY_INTERFACE);
    envBase.push_back(REQUEST_METHOD);
    envBase.push_back(REQUEST_URI);
    envBase.push_back(PATH_INFO);
    envBase.push_back(PATH_TRANSLATED);
    envBase.push_back(SCRIPT_NAME);
    envBase.push_back(SCRIPT_FILENAME);
    envBase.push_back(QUERY_STRING);
    envBase.push_back(SERVER_NAME);
    envBase.push_back(SERVER_PORT);
    envBase.push_back(REMOTE_ADDR);
    envBase.push_back(REMOTE_HOST);
    envBase.push_back(SERVER_SOFTWARE);
	envBase.push_back(HTTP_X_SECRET_HEADER_FOR_TEST);

    for (size_t i = 0; i < envBase.size(); i++)
	{
		m_envp.push_back(&envBase[i][0]);
	}
    m_envp.push_back(NULL);
	argvBase.push_back(path);
    for (size_t i = 0; i < argvBase.size(); i++)
	{
		m_argv.push_back(&argvBase[i][0]);
	}
	m_argv.push_back(NULL);
}

void
Cgi::executeCgi(int cgiToServer[2], int serverToCgi[2])
{
	// TODO: fix
	m_pid = fork();
	if (m_pid < 0)
	{
		throw std::runtime_error("Cgi::executeCgi() fork() fail");
	}
	else if (m_pid == 0)
	{
		close(serverToCgi[1]);
		close(cgiToServer[0]);
		if (dup2(serverToCgi[0], STDIN_FILENO) != STDIN_FILENO
			|| dup2(cgiToServer[1], STDOUT_FILENO) != STDOUT_FILENO)
		{
			throw std::runtime_error("Cgi::executeCgi() dup2() fail");
		}
		close(serverToCgi[0]);
		close(cgiToServer[1]);
		ServerManager::closeListenServer();
		Cgi::closePipeList();
		close(m_requestHandler->m_socket->m_fd);
		execve(m_cgiPath.c_str(), m_argv.data(), m_envp.data());
		throw std::runtime_error("Cgi::executeCgi() execve() fail");
	}
	close(serverToCgi[0]);
	close(cgiToServer[1]);
	s_cgiPipeList.push_back(serverToCgi[1]);
	s_cgiPipeList.push_back(cgiToServer[0]);
}

void
Cgi::executeCgi(int cgiToServer[2])
{
	// TODO: fix
	m_pid = fork();
	if (m_pid < 0)
	{
		throw std::runtime_error("Cgi::executeCgi() fork() fail");
	}
	else if (m_pid == 0)
	{
		close(cgiToServer[0]);
		if (dup2(cgiToServer[1], STDOUT_FILENO) != STDOUT_FILENO)
		{
			throw std::runtime_error("Cgi::executeCgi() dup2() fail");
		}
		close(cgiToServer[1]);
		ServerManager::closeListenServer();
		Cgi::closePipeList();
		close(m_requestHandler->m_socket->m_fd);
		execve(m_cgiPath.c_str(), m_argv.data(), m_envp.data());
		throw std::runtime_error("Cgi::executeCgi() execve() fail");
	}
	close(cgiToServer[1]);
	s_cgiPipeList.push_back(cgiToServer[0]);
}

void
Cgi::respondStatusLine(int statusCode)
{
	m_requestHandler->m_sendBuffer.append(g_httpVersion);
	m_requestHandler->m_sendBuffer.append(" ");
	m_requestHandler->m_sendBuffer.append(Util::toString(statusCode));
	m_requestHandler->m_sendBuffer.append(" ");
	m_requestHandler->m_sendBuffer.append(HttpErrorHandler::getErrorMessage(statusCode));
	m_requestHandler->m_sendBuffer.append(g_CRLF);
}

void
Cgi::respondHeader()
{
	m_requestHandler->m_sendBuffer.append("Server: webserv/2.0");
	m_requestHandler->m_sendBuffer.append(g_CRLF);
	m_requestHandler->m_sendBuffer.append("Date: " + Util::getDate("%a, %d %b %Y %X %Z"));
	m_requestHandler->m_sendBuffer.append(g_CRLF);
	m_requestHandler->m_sendBuffer.append(m_responseHeader);
}

void
Cgi::closePipeList()
{
	for (unsigned int i = 0; i < s_cgiPipeList.size(); ++i)
	{
		close(s_cgiPipeList[i]);
	}
}
