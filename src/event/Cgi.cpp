#include <sys/wait.h>
#include <functional>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <unistd.h>

#include "Logger.hpp"
#include "VirtualServer.hpp"
#include "http/RequestHandler.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "Cgi.hpp"
#include "ServerManager.hpp"

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
	m_totalCnt = 0;
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
			// LOG(INFO, "cgi read event at %d", m_fd);
			if (receiveCgiResponse() == 0) // fall through
			{
				// LOG(DEBUG, "cgi(fd=%d) termination", m_fd);
				// return IoEventPoller::STAT_END;
				return IoEventPoller::STAT_NORMAL;
			}
			return IoEventPoller::STAT_NORMAL;
		case IoEventPoller::FILT_WRITE:
			// LOG(INFO, "cgi write event at %d", m_fd);
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
	// static int totalCnt = 0;

	// LOG(INFO, "receiveCgiResponse() read before fromCgiBuffer size : %d", m_fromCgiBuffer.size());
	// cnt = read(m_fd, &m_fromCgiBuffer[0], 60000);
	cnt = m_fromCgiBuffer.receive(m_fd);
	// cout << m_fromCgiBuffer << endl;
	m_totalCnt += cnt;
	// LOG(INFO, "receiveCgiResponse() read after total count : %d cur count : %d", totalCnt, cnt);
	// LOG(INFO, "m_status : %d", m_status);
	// LOG(INFO, "cgi read event finished at %d", m_fd);
	// totalCnt += cnt;
	// LOG(INFO, "receiveCgiResponse() count = %d, cnt = %d", m_totalCnt, cnt);
	switch (m_status)
	{
		case Cgi::CGI_HEADER:
			statusCode = parseCgiHeader();
			if (m_status != CGI_CONTENT)
				break;
			respondStatusLine(statusCode);
			respondHeader();
			m_requestHandler->m_sendBuffer.append("Transfer-Encoding: chunked");
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			// NOTE: if buffer is empty after cgi header parsing, content start with 0\r\n\r\n
			if (m_fromCgiBuffer.size() == 0)
				break;
			// fall through
		case Cgi::CGI_CONTENT:
			m_requestHandler->m_sendBuffer.append(Util::toHex(m_fromCgiBuffer.size()));
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			m_requestHandler->m_sendBuffer.append(m_fromCgiBuffer);
			m_requestHandler->m_sendBuffer.append(g_CRLF);
			m_fromCgiBuffer.clear();
			// LOG(INFO, "receiveCgiResponse cnt  %d", cnt);
			if (cnt == 0)
			{
				// LOG(INFO, "cgi read EOF at %d", m_fd);
				m_requestHandler->resetStates();
				delete this;
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
	int	count = m_toCgiBuffer->mysend(m_serverToCgi[1]);
	static int clearCnt = 0;

	// static int totCnt = 0;
	// int beforeSize = 0;
	// int writeSize = 65000;

	// if (m_fd == 21 || m_fd == 22)
		// cout << m_toCgiBuffer->size() << endl;
	// if (m_toCgiBuffer->size() == 0)
	//     return 0;
	// LOG(INFO, "sendCgiRequest() write before size : %d", m_toCgiBuffer->size());
	// beforeSize = m_toCgiBuffer->size();
	// if (m_toCgiBuffer->size() <= 65000)
	//     writeSize = m_toCgiBuffer->size();
	// cout << m_toCgiBuffer->size() << endl;
	// int count = write(m_serverToCgi[1], m_toCgiBuffer->c_str(), 8192);
	// m_toCgiBuffer->erase(0, 8192);
	// totCnt = totCnt + beforeSize - m_toCgiBuffer->size();
	// cout << "cgi buffer : "<< m_toCgiBuffer->size() << " cout : " << count << " total cnt : " << totCnt << endl;
	// m_toCgiBuffer->erase(0, count);
	// if (count != 0)
	// {
	//     // LOG(DEBUG, "sendCgiRequest() count = %d", count);
	//     return count;
	// }
	if (m_toCgiBuffer->status() == Buffer::BUF_EOF && m_toCgiBuffer->size() == 0)
	{
		// LOG(INFO, "sendCgiRequest() send eof");
		cout << ++clearCnt << endl;
		close(m_serverToCgi[1]);
		return (-1);
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
