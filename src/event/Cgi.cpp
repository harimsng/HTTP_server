#include <sys/wait.h>

#include <functional>
#include <stdexcept>
#include <unistd.h>
#include <iostream>

#include "Logger.hpp"
#include "VirtualServer.hpp"
#include "http/RequestHandler.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "Cgi.hpp"

using namespace std;

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
//	m_serverToCgi(serverToCgi),
//	m_cgiToServer(cgiToServer),
	m_toCgiBuffer(&toCgiBuffer),
	m_status(CGI_HEADER)
{
	m_serverToCgi[0] = serverToCgi[0];
	m_serverToCgi[1] = serverToCgi[1];
	m_cgiToServer[0] = cgiToServer[0];
	m_cgiToServer[1] = cgiToServer[1];
}

Cgi::Cgi(int cgiToServer[2], int serverToCgi[2], RequestHandler& requestHandler, Buffer& toCgiBuffer, int for_write)
:	EventObject(serverToCgi[1]),
	m_requestHandler(&requestHandler),
//	m_serverToCgi(serverToCgi),
//	m_cgiToServer(cgiToServer),
	m_toCgiBuffer(&toCgiBuffer),
	m_status(CGI_HEADER)
{
	(void)for_write;
	m_serverToCgi[0] = serverToCgi[0];
	m_serverToCgi[1] = serverToCgi[1];
	m_cgiToServer[0] = cgiToServer[0];
	m_cgiToServer[1] = cgiToServer[1];
}

Cgi::Cgi(int fileFd, int writeEnd, RequestHandler& requestHandler)
:	m_requestHandler(&requestHandler),
	m_requestContentFileFd(fileFd)
{
	(void) writeEnd;
}

Cgi::~Cgi()
{
	//int	status;

	//close(m_fd);
	//waitpid(m_pid, &status, WNOHANG); // zero sized receive from pipe guarantee that cgi has exited.
}

Cgi::IoEventPoller::EventStatus
Cgi::handleEventWork()
{
	switch (m_filter)
	{
		case IoEventPoller::FILT_READ:
			if (receiveCgiResponse() == 0) // fall through
			{
				// LOG(DEBUG, "cgi(fd=%d) termination", m_fd);
				return IoEventPoller::STAT_END;
			}
			return IoEventPoller::STAT_NORMAL;
		case IoEventPoller::FILT_WRITE:
			// // LOG(DEBUG, "cgi write event");
			// Cgi must be terminated when connection is lost
			if (sendCgiRequest() == -1)
				return IoEventPoller::STAT_NORMAL;
			return IoEventPoller::STAT_NORMAL;
		default:
			throw std::runtime_error("not handled event filter in Cgi::handleEvent()");
	}
	return IoEventPoller::STAT_NORMAL;
}

int
Cgi::receiveCgiResponse()
{
	int cnt;
	int statusCode;

	usleep(100);
	cnt = m_fromCgiBuffer.receive(m_fd);
	if (cnt == 0)
	{
		statusCode = parseCgiHeader();
		respondStatusLine(statusCode);
		respondHeader();
		m_requestHandler->m_sendBuffer.append("Content-Length: ");
		cout << m_responseBody.size() << endl;
		m_requestHandler->m_sendBuffer.append(Util::toString(m_responseBody.size()));
		m_requestHandler->m_sendBuffer.append(g_CRLF);
		m_requestHandler->m_sendBuffer.append(g_CRLF);
		m_requestHandler->m_sendBuffer.append(m_responseBody);
		m_fromCgiBuffer.clear();
		close(m_fd);
		m_requestHandler->resetStates();
		return (0);
	}
	else
	{
		m_responseBody += m_fromCgiBuffer;
		m_fromCgiBuffer.clear();
		return (1);
	}
}

// if output is slower than input, the buffer grows up indefinitely.
int
Cgi::sendCgiRequest()
{
	// NOTE: there's chance for blocking because we don't know pipe memory left.
	
	int	count = m_toCgiBuffer->send(m_serverToCgi[1]);
	if (count != 0)
	{
//		// LOG(DEBUG, "sendCgiRequest() count = %d", count);
		return count;
	}
	if (m_toCgiBuffer->status() == Buffer::BUF_EOF)
	{
		close(m_serverToCgi[1]);
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

	if (m_responseBody.find("\r\n\r\n") == string::npos)
		return statusCode;

	while (1)
	{
		end = m_responseBody.find(g_CRLF, start);
		headerField = m_responseBody.substr(start, end - start);
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
	m_responseBody.erase(0, end + 2);
	m_status = CGI_CONTENT;
	return (statusCode);
}

void
Cgi::initEnv(const Request &request)
{
	std::string 				m_path;
	std::string					m_script;
	std::string					m_query;
	std::string					ext;

	// INFO: dangerous
	ext = request.m_file.substr(request.m_file.rfind("."));
	m_cgiPath = request.m_virtualServer->m_cgiPass[ext];
	m_path = request.m_path + request.m_file;
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
    std::string SCRIPT_NAME = "SCRIPT_NAME=" + request.m_file; //+ request.m_locationBlock->m_cgiPass;
    std::string SCRIPT_FILENAME = "SCRIPT_FILENAME=" + request.m_path + request.m_file; // path of cgi script in file-system
    std::string QUERY_STRING = "QUERY_STRING=";
    if (request.m_method == RequestHandler::GET)
    {
        QUERY_STRING += request.m_queryString;
	}
	m_env.reserve(32);
	m_envp.reserve(32);
	m_argv.reserve(32);
    m_env.push_back(REDIRECT_STATUS);
    m_env.push_back(CONTENT_LENGTH);
    m_env.push_back(CONTENT_TYPE);
    m_env.push_back(SERVER_PROTOCOL);
    m_env.push_back(GATEWAY_INTERFACE);
    m_env.push_back(REQUEST_METHOD);
    m_env.push_back(REQUEST_URI);
    m_env.push_back(PATH_INFO);
    m_env.push_back(PATH_TRANSLATED);
    m_env.push_back(SCRIPT_NAME);
    m_env.push_back(SCRIPT_FILENAME);
    m_env.push_back(QUERY_STRING);
    m_env.push_back(SERVER_NAME);
    m_env.push_back(SERVER_PORT);
    m_env.push_back(REMOTE_ADDR);
    m_env.push_back(REMOTE_HOST);
    m_env.push_back(SERVER_SOFTWARE);
	m_env.push_back(HTTP_X_SECRET_HEADER_FOR_TEST);

    for (size_t i = 0; i < m_env.size(); i++)
	{
		m_envp.push_back(&m_env[i][0]);
	}
    m_envp.push_back(NULL);
	m_argvBase.push_back(m_path);
    for (size_t i = 0; i < m_argvBase.size(); i++)
	{
		m_argv.push_back(&m_argvBase[i][0]);
	}
	m_argv.push_back(NULL);
}

void
Cgi::executeCgi(int pipe[2], std::string& readBody, const Request &request)
{
//  TODO: close when cgi is done
//	close(m_fd);
	struct stat st;

	m_pid = fork();
	if (m_pid < 0)
	{
		throw std::runtime_error("Cgi::Cgi() fork failed");
	}
	if (m_pid == 0)
	{
		// Child process
		lseek(m_requestContentFileFd, 0, SEEK_SET);
		close(pipe[1]);
		dup2(pipe[0], STDIN_FILENO);

		dup2(m_requestContentFileFd, STDOUT_FILENO);
		close(pipe[0]);
		execve(m_cgiPath.c_str(), m_argv.data(), m_envp.data());
		throw std::runtime_error("Cgi::Cgi() execve failed");
	}
		// Parent process
	close(pipe[0]);

	// WARNING: if (request.m_bodySize > size of pipe buffer(usually 65536 bytes)), written size = size of pipe buffer.
	if (request.m_bodySize != 0 && write(pipe[1], request.requestBodyBuf.c_str(), request.m_bodySize) <= 0)
		return;
	close(pipe[1]);
	int status;
	pid_t wpid = waitpid(m_pid, &status, 0);
	if (wpid == -1)
		return;
	lseek(m_requestContentFileFd, 0, SEEK_SET);
	fstat(m_requestContentFileFd, &st);
	off_t fileSize = st.st_size;
	// LOG(DEBUG, "filesize = %d", fileSize);
	readBody.resize(fileSize, 0);
	// FIX: casting const pointer to normal pointer is UB
	read(m_requestContentFileFd, &readBody[0], fileSize);
	close(m_requestContentFileFd);
	readBody = readBody.substr(readBody.find("\r\n\r\n") + 4);
}

void
Cgi::executeCgi()
{
	m_pid = fork();
	if (m_pid < 0)
	{
		throw std::runtime_error("Cgi::executeCgi() fork() fail");
	}
	else if (m_pid == 0)
	{
		// child process
		// NOTE: do fd leaks block creating more cgi?
		close(m_requestHandler->m_socket->m_fd);
		close(m_serverToCgi[1]);
		close(m_cgiToServer[0]);
		if (dup2(m_serverToCgi[0], STDIN_FILENO) != STDIN_FILENO
			|| dup2(m_cgiToServer[1], STDOUT_FILENO) != STDOUT_FILENO)
		{
			throw std::runtime_error("Cgi::executeCgi() dup2() fail");
		}
		close(m_serverToCgi[0]);
		close(m_cgiToServer[1]);
		execve(m_cgiPath.c_str(), m_argv.data(), m_envp.data());
		throw std::runtime_error("Cgi::executeCgi() execve() fail");
	}
	close(m_serverToCgi[0]);
	close(m_cgiToServer[1]);
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
// #endif
