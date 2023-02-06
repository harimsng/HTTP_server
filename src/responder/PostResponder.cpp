#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <vector>

#include "ServerManager.hpp"
#include "io/IoMultiplex.hpp"
#include "event/Cgi.hpp"
#include "util/Util.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "PostResponder.hpp"

using namespace std;

extern const std::string	g_tempDir;

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
#ifndef TEST
	std::string	readBody;
	std::string tmpFile = g_tempDir + m_request.m_file;
#endif
	struct stat st;

	if (m_request.m_status >= 300)
		throw (m_request.m_status);
	switch (m_responseStatus)
	{
		case RES_HEADER:
			openFile(g_tempDir + m_request.m_file);
			m_responseStatus = RES_CONTENT;
		case RES_CONTENT:
			// TODO
			// cgi 만듦 -> fd 생성
			// cgi readEvent 등록
			// cgi 돌림
			if (!(this->*m_recvContentFunc)())
				break;
			m_responseStatus = RES_CONTENT_FINISHED;
		case RES_CONTENT_FINISHED:
#ifndef TEST
			lseek(m_fileFd, 0, SEEK_SET);
			fstat(m_fileFd, &st);
			m_request.m_bodySize = st.st_size;
			m_request.requestBodyBuf.resize(m_request.m_bodySize , 0);
			// FIX: casting const pointer to normal pointer is UB
			read(m_fileFd, &m_request.requestBodyBuf[0], m_request.m_bodySize);
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
#else
			if (m_request.m_isCgi == true)
			{
				openFile(g_tempDir + m_request.m_file);
				constructCgi();
				m_responseStatus = RES_RECV_CGI;
				break;
			}
			string	readBody;
			readFile(readBody);
			respondStatusLine(200);
			respondHeader();
			respondBody(readBody);
			m_responseStatus = RES_DONE;
#endif
		case RES_RECV_CGI:
			m_responseStatus = RES_DONE;
		case RES_DONE:
			close(m_fileFd);
			endResponse();
		default:
			;
	}
}
catch(int errorStatusCode)
{
	string readBody;

	m_request.m_status = errorStatusCode;
	respondStatusLine(errorStatusCode);
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
#ifndef TEST
	m_cgiReadEnd = pipeSet[0];
#endif

	Cgi*	cgi = new Cgi(m_fileFd, pipeSet[1], m_requestHandler);

#ifdef TEST
	ServerManager::registerEvent(pipeSet[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
	cgi->initEnv(m_request);
	cgi->executeCgi(pipeSet);
#endif
	// NOTE: deallcation
	cgi->initEnv(m_request);
	cgi->executeCgi(pipeSet, readBody, m_request);
#ifdef TEST
	m_responseStatus = RES_RECV_CGI;
#endif
}
