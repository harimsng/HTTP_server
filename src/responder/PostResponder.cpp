#include <unistd.h>
#include <fcntl.h>
#include <vector>

#include "event/Cgi.hpp"
#include "ServerManager.hpp"
#include "PostResponder.hpp"

using namespace std;

extern const std::string	g_tempDir;

const PostResponder::t_transition	PostResponder::s_transitionTable[5] = {
	&PostResponder::header,
	&PostResponder::recvContent,
	&PostResponder::recvContentDone,
	&PostResponder::sendToCgi,
	&PostResponder::done,
};

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

////////////////////////////////////////
////////////////////////////////////////

void
PostResponder::respondWork()
{
	RESPONDER_TRANSITION(m_responseState);
}

void
PostResponder::header()
{
	if (m_request.m_isCgi == true)
	{
		constructCgi();
		RESPONDER_TRANSITION(RES_SEND_TO_CGI);
	}
	else
	{
		// NOTE: test when isCgi is false.
		openFile(g_tempDir + m_request.m_file);
		RESPONDER_TRANSITION(RES_RECV_CONTENT);
	}
}

void
PostResponder::recvContent()
{
	if ((this->*m_recvContentFunc)() == 0)
		return;
	RESPONDER_TRANSITION(RES_RECV_CONTENT_DONE);
}

void
PostResponder::recvContentDone()
{
	string	readBody;

	readFile(readBody);
	respondStatusLine(200);
	respondHeader();
	respondBody(readBody);
	RESPONDER_TRANSITION(RES_DONE);
}

void
PostResponder::sendToCgi()
{
	(this->*m_recvContentFunc)();
	return; // Cgi determine end of response.
}

void
PostResponder::done()
{
	endResponse();
	return;
}

////////////////////////////////////////
////////////////////////////////////////

void
PostResponder::constructCgi()
{
	int cgiToServer[2];// cgi의 stdout
	int serverToCgi[2]; // cgi의 stdin

	if (pipe(cgiToServer) < 0
		|| pipe(serverToCgi) < 0)
		throw runtime_error("pipe fail in PostRedponder::contructCgi()");

//	fcntl(serverToCgi[1], F_SETFL, O_NONBLOCK);
//	m_fileFd = serverToCgi[1];

	Cgi*	cgi = new Cgi(cgiToServer, serverToCgi, m_requestHandler, m_buffer);
	(void)cgi;
}

/*
	std::string	readBody;
	std::string tmpFile = g_tempDir + m_request.m_file;

	if (m_request.m_status >= 300)
		throw (m_request.m_status);

	switch (m_responseState)
	{
		case RES_HEADER:
			if (m_request.m_isCgi == true)
			{
				constructCgi();
				m_responseState = RES_SEND_TO_CGI;
			}
			else
			{
				// NOTE: test when isCgi is false
				openFile(tmpFile);
				m_responseState = RES_RECV_CONTENT;
			}
 // fall through
		case RES_RECV_CONTENT:
			if (!(this->*m_recvContentFunc)())
				break;
			m_responseState = RES_RECV_CONTENT_FINISHED;
// fall through
		case RES_RECV_CONTENT_FINISHED:
			if (m_request.m_isCgi == true)
				break;
			readFile(readBody);
			respondStatusLine(200);
			respondHeader();
			respondBody(readBody);
			m_responseState = RES_DONE;
				// early close possiblity. m_fileFd is closed right after receiving request content has finished.
				// close(m_fileFd);
				// break here for cgi to finializes
// fall through
		case RES_DONE:
//			close(m_fileFd);
			endResponse();
			break;
		case RES_SEND_TO_CGI:
			if (!(this->*m_recvContentFunc)())
				break;
			break;
		default:
			;
	}
}
*/
