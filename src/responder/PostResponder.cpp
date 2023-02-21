#include <sys/socket.h>
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
	std::string	readBody;
	std::string tmpFile = m_request.m_path + m_request.m_file + ".tmp";

	if (m_request.m_status >= 300)
		throw (m_request.m_status);
	switch (m_responseStatus)
	{
		case RES_HEADER:
			if (m_request.m_isCgi != true)
				openFile(tmpFile);
			m_responseStatus = RES_CONTENT; // fall through
		case RES_CONTENT:
			if (!(this->*m_recvContentFunc)())
				break;
			m_responseStatus = RES_CONTENT_FINISHED; // fall through
		case RES_CONTENT_FINISHED:
			if (m_request.m_isCgi == true)
			{
				constructCgi();
				break;
			}
			readFile(readBody);
			respondStatusLine(200);
			respondHeader();
			respondBody(readBody);
			m_responseStatus = RES_DONE;
			// fall through
		case RES_RECV_CGI:
			m_responseStatus = RES_DONE;
			// fall through
		case RES_DONE:
			endResponse();
			break;
		default:
			;
	}
	if (m_request.m_isCgi == false)
	{
		unlink(tmpFile.c_str());
		close(m_fileFd);
	}
}

void
PostResponder::constructCgi()
{
	int cgiToServer[2];
	int serverToCgi[2];


	if (pipe(cgiToServer) < 0
		|| pipe(serverToCgi) < 0)
		throw runtime_error("pipe fail in PostRedponder::contructCgi()");

	fcntl(serverToCgi[1], F_SETFL, O_NONBLOCK);

	Cgi*	cgi = new Cgi(cgiToServer, serverToCgi, m_requestHandler, m_buffer);
	ServerManager::registerEvent(cgiToServer[0], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
	ServerManager::registerEvent(serverToCgi[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_WRITE, cgi);
	cgi->initEnv(m_request);
	cgi->executeCgi();
}
