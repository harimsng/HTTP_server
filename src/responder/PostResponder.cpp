#include <unistd.h>
#include <fcntl.h>
#include <vector>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "io/IoMultiplex.hpp"
#include "event/Cgi.hpp"
#include "tokenizer/HttpStreamTokenizer.hpp"
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
PostResponder::respondWork()
{
#ifndef TEST
	std::string	readBody;
	std::string tmpFile = g_tempDir + m_request.m_file;

#endif
	// struct stat st;

	if (m_request.m_status >= 300)
		throw (m_request.m_status);
	switch (m_responseStatus)
	{
		case RES_HEADER:
			if (m_request.m_isCgi == true)
				constructCgi();
			else
				openFile(tmpFile);
			m_responseStatus = RES_CONTENT; // fall through
		case RES_CONTENT:
			if (!(this->*m_recvContentFunc)())
				break;
			m_responseStatus = RES_CONTENT_FINISHED; // fall through
		case RES_CONTENT_FINISHED:
			if (m_request.m_isCgi == true)
				break;
			readFile(readBody);
			respondStatusLine(200);
			respondHeader();
			respondBody(readBody);
			m_responseStatus = RES_DONE;
				// early close possiblity. m_fileFd is closed right after receiving request content has finished.
				// close(m_fileFd);
				// break here for cgi to finializes
			// fall through
		case RES_RECV_CGI:
			m_responseStatus = RES_DONE;
			// fall through
		case RES_DONE:
//			close(m_fileFd);
			endResponse();
			break;
		default:
			;
	}
}

// void
// PostResponder::constructCgi(std::string& readBody)
// {
//     int	pipeSet[2];
//
//     // int fileFd[2];
//
//     pipe(pipeSet);
// #ifndef TEST
//     m_cgiReadEnd = pipeSet[0];
// #endif
//     // pipeSet[0]  ->
//     // pipeSet[1]
//
// //						cgi 리턴하는곳	안씀
//     Cgi*	cgi = new Cgi(m_fileFd, pipeSet[1], m_requestHandler);
//
// #ifdef TEST
//     ServerManager::registerEvent(pipeSet[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
//     cgi->initEnv(m_request);
//     cgi->executeCgi(pipeSet);
// #endif
//     // NOTE: deallcation
//     cgi->initEnv(m_request);
//     cgi->executeCgi(pipeSet, readBody, m_request);
// #ifdef TEST
//     m_responseStatus = RES_RECV_CGI;
// #endif
// }

void
PostResponder::constructCgi()
{
	int cgiToServer[2];// cgi의 stdout
	int serverToCgi[2]; // cgi의 stdin

	if (pipe(cgiToServer) < 0
		|| pipe(serverToCgi) < 0)
		throw runtime_error("pipe fail in PostRedponder::contructCgi()");

	fcntl(serverToCgi[1], F_SETFL, O_NONBLOCK);

//	m_fileFd = serverToCgi[1];

	Cgi*	cgi = new Cgi(cgiToServer, serverToCgi, m_requestHandler, m_buffer);
	ServerManager::registerEvent(cgiToServer[0], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
	ServerManager::registerEvent(serverToCgi[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_WRITE, cgi);
	cgi->initEnv(m_request);
	cgi->executeCgi();
}
