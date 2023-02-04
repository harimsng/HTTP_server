#include <unistd.h>
#include <iostream>
#include "ServerManager.hpp"
#include "io/IoMultiplex.hpp"
#include "event/Cgi.hpp"
#include "PostResponder.hpp"

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
PostResponder::respond()
{
	std::string	readBody;

	switch (m_responseStatus)
	{
		case RES_HEADER:
			openFile(m_request.m_path + m_request.m_file + ".temp");
			respondHeader(); // fall through
		case RES_CONTENT:
			readRequestBody();
			std::cout << "recvbuf = " << m_recvBuffer << std::endl;
			if (m_responseStatus != RES_DONE)
				break; // fall through
		case RES_CONTENT_FINISHED:
			constructCgi(readBody);
			break;
		case RES_RECV_CGI:
		case RES_DONE:
			endResponse();
		default:
			;
	}
}

void
PostResponder::constructCgi(std::string& readBody)
{
	int	pipeSet[2];

	pipe(pipeSet);
	m_cgiReadEnd = pipeSet[0];

	Cgi*	cgi = new Cgi(m_fileFd, pipeSet[1], m_requestHandler);

	ServerManager::registerEvent(pipeSet[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
	cgi->initEnv(m_request);
	cgi->executeCgi(pipeSet, readBody);
	m_responseStatus = RES_RECV_CGI;
}

/*
 * event notified
 *
 * read
 * 		server
 * 				accept()
 * 		client
 *				requestHandler
 *				clientSocket ->   receiveRequest()
 *				parsed request -> createResponseHeader()
 *				method
 *				clientSocket ->	  method->respond()
				
 *										respond
 *
 * write
 */
