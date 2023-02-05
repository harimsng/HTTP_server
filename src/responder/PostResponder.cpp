#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include "ServerManager.hpp"
#include "io/IoMultiplex.hpp"
#include "event/Cgi.hpp"
#include "util/Util.hpp"
#include "PostResponder.hpp"
#include "exception/HttpErrorHandler.hpp"

using namespace std;

// constructors & destructor
PostResponder::PostResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}
PostResponder::~PostResponder()
{
}

// operators
PostResponder&
PostResponder::operator=(const PostResponder& postResponder)
{
	(void)postResponder;
	return *this;
}

void
PostResponder::respond() try
{
	std::string	readBody;
	std::string tmpFile = "/Users/gson/goinfre/" + m_request.m_file + ".temp" + Util::toString(m_requestHandler.m_socket->m_fd);
	struct stat st;

	if (m_request.m_status >= 300)
		throw (m_request.m_status);
	switch (m_responseStatus)
	{
		case RES_HEADER:
			openFile(tmpFile);
			m_responseStatus = RES_CONTENT;
		case RES_CONTENT:
			// TODO
			// cgi 만듦 -> fd 생성
			// cgi readEvent 등록
			// cgi 돌림
			if (!readRequestBody())
				break;
			m_responseStatus = RES_CONTENT_FINISHED;
		case RES_CONTENT_FINISHED:
			lseek(m_fileFd, 0, SEEK_SET);
			fstat(m_fileFd, &st);
			m_request.m_bodySize = st.st_size;
			m_request.requestBodyBuf.resize(m_request.m_bodySize , 0);
			read(m_fileFd, (char *)(m_request.requestBodyBuf.data()), m_request.m_bodySize);
			unlink(tmpFile.c_str());
			if (m_request.m_isCgi == true)
			{
				openFile(tmpFile);
				constructCgi(readBody);
			}
			else
				readFile(readBody);
			unlink(tmpFile.c_str());
			respondStatusLine(200);
			respondHeader();
			respondBody(readBody);
			m_responseStatus = RES_DONE;
		/*
		case RES_RECV_CGI:
		*/
		case RES_DONE:
			endResponse();
		default:
			;
	}
}
catch(int ErrorstatusCode)
{
	string readBody;

	m_request.m_status = ErrorstatusCode;
	respondStatusLine(ErrorstatusCode);
	respondHeader();
	m_request.m_file.clear();
	m_request.m_path = getErrorPage(readBody);
	readFile(readBody);
	respondBody(readBody);
	endResponse();
}

void
PostResponder::constructCgi(std::string& readBody)
{
	int	pipeSet[2];

	pipe(pipeSet);
	m_cgiReadEnd = pipeSet[0];

	Cgi*	cgi = new Cgi(m_fileFd, pipeSet[1], m_requestHandler);

	//ServerManager::registerEvent(pipeSet[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
	cgi->initEnv(m_request);
	cgi->executeCgi(pipeSet, readBody, m_request);
	// m_responseStatus = RES_RECV_CGI;
}
