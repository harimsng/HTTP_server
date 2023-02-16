#include <fcntl.h>

#include "Logger.hpp"
#include "VirtualServer.hpp"
#include "ServerManager.hpp"
#include "io/IoMultiplex.hpp"
#include "event/Cgi.hpp"
#include "http/AutoIndex.hpp"
#include "util/Util.hpp"
#include "responder/GetResponder.hpp"

using namespace std;

extern string	g_tempDir;

const GetResponder::t_transition	GetResponder::s_transitionTable[5] = {
	&GetResponder::header,
	&GetResponder::recvContent,
	NULL, // recvContentDone
	NULL, // sendToCgi
	&GetResponder::done,
};


// constructors & destructor
GetResponder::GetResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}

GetResponder::~GetResponder()
{
}

// operators
GetResponder&
GetResponder::operator=(const GetResponder& getMethod)
{
	(void)getMethod;
	return *this;
}

////////////////////////////////////////
////////////////////////////////////////

void
GetResponder::respondWork()
{
	RESPONDER_TRANSITION(m_responseState);
}

void
GetResponder::header()
{
	// NOTE: add '&& m_request.m_status == 404'?
	if (isAutoIndex() == true && m_request.m_status == 404)
		m_request.m_status = 200;
	respondStatusLine(m_request.m_status);
	respondHeader();
	RESPONDER_TRANSITION(RES_RECV_CONTENT);
}

void
GetResponder::recvContent()
{
	string	readBody;

	if (m_request.m_isCgi == true)
	{
		constructCgi();
		return;
	}
	if (isAutoIndex() == true)
	{
		readBody = AutoIndex::autoIndex(m_request.m_path, m_request.m_uri);
	}
	else if (m_request.m_status != 200)
	{
		m_request.m_path = getErrorPage(readBody);
	}
	readFile(readBody);
	respondBody(readBody);
	RESPONDER_TRANSITION(RES_DONE);
}

void
GetResponder::done()
{
	endResponse();
	return;
}

////////////////////////////////////////
////////////////////////////////////////

void
GetResponder::constructCgi()
{
	int	cgiToServer[2];

	if (pipe(cgiToServer) < 0)
		throw std::runtime_error("pipe() fail in GetResponder::constructCgi()");

	Cgi*	cgi = new Cgi(cgiToServer, m_requestHandler);

	(void)cgi;
}

bool
GetResponder::isAutoIndex()
{
	if (m_request.m_file == "")
	{
		if (m_request.m_locationBlock != NULL)
			return (m_request.m_locationBlock->m_autoindex);
		return (m_request.m_virtualServer->m_autoindex);
	}
	return (false);
}

/*
void
GetResponder::respondWork()
{
	string readBody;

	switch (m_responseState)
	{
		case RES_HEADER:
			respondStatusLine(m_request.m_status);
			respondHeader();
			m_responseState = RES_RECV_CONTENT; // fall through
// fall through
		case RES_RECV_CONTENT:
			if (isAutoIndex())
				readBody = AutoIndex::autoIndex(m_request.m_path, m_request.m_uri);
			else if (m_request.m_isCgi == true)
			{
				constructCgi();
				break;
			}
			else if (m_request.m_status != 200)
			{
				m_request.m_path = getErrorPage(readBody);
			}
			readFile(readBody);
			respondBody(readBody);
			m_responseState = RES_DONE;
			// break;
// fall through
		case RES_DONE:
		// method must know end of response(content length, chunked)
			endResponse();
			break;
		default:
			;
	}
}
*/
