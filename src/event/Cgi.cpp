#include <iostream>
#include <stdexcept>

#include "Logger.hpp"
#include "VirtualServer.hpp"
#include "http/RequestHandler.hpp"
#include "Cgi.hpp"



#include <iostream>

using namespace std;

#define BUFFER_SIZE (65535)

// deleted
Cgi&	Cgi::operator=(Cgi const& cgi)
{
	(void)cgi;

	return *this;
}

Cgi::Cgi(Cgi const& cgi)
//:	EventObject()
{
	(void)cgi;
}

// constructors & destructor
Cgi::Cgi(int fileFd, int writeEnd, RequestHandler& requestHandler)
:	m_requestHandler(&requestHandler),
	m_requestContentFileFd(fileFd)
{
	(void) writeEnd;
	m_bodyFlag = false;
}

Cgi::~Cgi()
{
}

/*
Cgi::IoEventPoller::EventStatus
Cgi::handleEventWork()
{
	switch (m_filter)
	{
		case IoEventPoller::FILT_READ:
			receiveCgiResponse(); // fall through
		case IoEventPoller::FILT_WRITE:
			return IoEventPoller::STAT_ERROR;
		default:
			throw std::runtime_error("not handled event filter in Cgi::handleEvent()");
	}
	return IoEventPoller::STAT_NORMAL;
}
*/

void
Cgi::receiveCgiResponse()
{
	// INFO: temporary function
	Buffer&	sendBuffer = m_requestHandler->m_sendBuffer;

	(void) sendBuffer;
	//sendBuffer.receive(m_readEnd);
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
    std::string CONTENT_LENGTH = "CONTENT_LENGTH=";
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
		// LOG(DEBUG, "secret header : \"%s\"", HTTP_X_SECRET_HEADER_FOR_TEST.c_str());
    }
    if (request.m_method == RequestHandler::POST && request.m_bodySize > 0)
    {
		//TODO: change to_string
        CONTENT_LENGTH += std::to_string(request.m_bodySize);
    }
    std::string SERVER_SOFTWARE = "SERVER_SOFTWARE=webserv/2.0";
    std::string SERVER_PROTOCOL = "SERVER_PROTOCOL=HTTP/1.1"; // different GET POST
    std::string GATEWAY_INTERFACE = "GATEWAY_INTERFACE=CGI/1.1";

    std::string SERVER_PORT = "SERVER_PORT=";
    std::string SERVER_NAME = "SERVER_NAME=";
    std::string REMOTE_ADDR = "REMOTE_ADDR=";
    std::string REMOTE_HOST = "REMOTE_HOST=";

    std::string REDIRECT_STATUS = "REDIRECT_STATUS=200"; // php-cgi direct exec
//    std::string HTTP_HOST = "HTTP_HOST="; == SERVER_NAME
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
	/*
	int	pid = fork();

	m_readEnd = pipe[0];
	m_fd = pipe[1];
	if (pid < 0)
		// 500
		throw std::runtime_error("Cgi::Cgi() fork failed");
	if (pid == 0)
	{
		close(m_readEnd);
		dup2(m_requestContentFileFd, STDIN_FILENO);
		dup2(m_fd, STDOUT_FILENO);
		LOG(DEBUG, "cgi path = \"%s\"", m_cgiPath.c_str());
		execve(m_cgiPath.c_str(), m_argv.data(), m_envp.data());
		throw std::runtime_error("Cgi::Cgi() execve failed");
	}
	else
	{

	}
//  TODO: close when cgi is done
//	close(m_fd);
	*/
    pid_t pid;
	m_readEnd = pipe[0];
	struct stat st;

    pid = fork();
    if (pid < 0)
	{
        throw std::runtime_error("Cgi::Cgi() fork failed");
    }
    if (pid == 0)
	{
        // Child process
		lseek(m_requestContentFileFd, 0, SEEK_SET);
		close(pipe[1]);
		dup2(pipe[0], STDIN_FILENO);
        dup2(m_requestContentFileFd, STDOUT_FILENO);
		close(pipe[0]);
		execve(m_cgiPath.c_str(), m_argv.data(), m_envp.data());
		throw std::runtime_error("Cgi::Cgi() execve failed");
    } else if (pid > 0) {
        // Parent process
		close(pipe[0]);
		if (request.m_bodySize && write(pipe[1], request.requestBodyBuf.c_str(), request.m_bodySize) <= 0)
			return;
		close(pipe[1]);
		int status;
		pid_t wpid = waitpid(pid, &status, 0);
		if (wpid == -1)
			return;
    }

	lseek(m_requestContentFileFd, 0, SEEK_SET);
	fstat(m_requestContentFileFd, &st);
	off_t fileSize = st.st_size;
	LOG(DEBUG, "filesize = %d", fileSize);
	readBody.resize(fileSize, 0);
	read(m_requestContentFileFd, (char *)(readBody.data()), fileSize);
	close(m_requestContentFileFd);
	readBody = readBody.substr(readBody.find("\r\n\r\n") + 4);
}
