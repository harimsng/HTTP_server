#include "PutResponder.hpp"

using namespace std;

const PutResponder::t_transition	PutResponder::s_transitionTable[5] = {
	&PutResponder::header,
	&PutResponder::recvContent,
	&PutResponder::recvContentDone,
	NULL,
	&PutResponder::done
};

// constructors & destructor
PutResponder::PutResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}

PutResponder::~PutResponder()
{
}

// operators
PutResponder&
PutResponder::operator=(const PutResponder& putResponder)
{
	(void)putResponder;
	return *this;
}

////////////////////////////////////////
////////////////////////////////////////

void
PutResponder::respondWork()
{
	RESPONDER_TRANSITION(m_responseState);
}

void
PutResponder::header()
{
	openFile();
	RESPONDER_TRANSITION(RES_RECV_CONTENT);
}

void
PutResponder::recvContent()
{
	if ((this->*m_recvContentFunc)() == 0)
		return;
	RESPONDER_TRANSITION(RES_RECV_CONTENT_DONE);
}

void
PutResponder::recvContentDone()
{
	string	readBody;

	respondStatusLine(201);
	respondHeader();
	m_request.m_file.clear();
	m_request.m_path = getErrorPage(readBody);
	readFile(readBody);
	respondBody(readBody);
	RESPONDER_TRANSITION(RES_DONE);
}

void
PutResponder::done()
{
	close(m_fileFd);
	endResponse();
	return;
}

////////////////////////////////////////
////////////////////////////////////////

/*
void
PutResponder::respondWork()
{
	if (m_request.m_status >= 300)
		throw (m_request.m_status);
	switch (m_responseState)
	{
		case RES_HEADER:
			openFile();
			m_responseState = RES_RECV_CONTENT;
// fall through
		case RES_RECV_CONTENT:
			if (!(this->*m_recvContentFunc)())
				break;
			// NOTE: doesn't PUT need CGI?
			m_responseState = RES_RECV_CONTENT_DONE;// -> client_max_body_size error
// fall through
		case RES_RECV_CONTENT_DONE:
			respondStatusLine(201);
			respondHeader();
			m_request.m_file.clear();
			m_request.m_path = getErrorPage(readBody);
			readFile(readBody);
			respondBody(readBody);
			throw (201); // fall through
		case RES_DONE:
			close(m_fileFd);
			endResponse();
			break;
		default:
			;
	}
}
*/
