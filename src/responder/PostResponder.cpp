#include <unistd.h>
#include <iostream>
#include "ServerManager.hpp"
#include "io/IoMultiplex.hpp"
#include "event/Cgi.hpp"
#include "util/Util.hpp"
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
	std::string tmpFile = m_request.m_path + m_request.m_file + ".temp";
	struct stat st;

	switch (m_responseStatus)
	{
		case RES_HEADER:
			openFile(tmpFile);
			respondHeader(); // fall through
		case RES_CONTENT:
			if (m_request.m_headerFieldsMap.count("CONTENT-LENGTH") > 0)
				normalReadBody();
			else
				chunkedReadBody();
			m_request.m_status = 201;
			if (m_request.m_isCgi == true)
			{
				lseek(m_fileFd, 0, SEEK_SET);
				fstat(m_fileFd, &st);
				m_request.m_bodySize = st.st_size;
				m_request.requestBodyBuf.resize(m_request.m_bodySize , 0);
				read(m_fileFd, (char *)(m_request.requestBodyBuf.data()), m_request.m_bodySize);
				openFile(tmpFile);
				constructCgi(readBody);
				//unlink(tmpFile.c_str());
			}
			else
				readFile(readBody);
			m_sendBuffer.append("Content-Length: ");
			m_sendBuffer.append(Util::toString(readBody.size()));
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.reserve(m_sendBuffer.size() + readBody.size());
			m_sendBuffer.append(readBody);
			m_responseStatus = RES_DONE;
			if (m_responseStatus != RES_DONE)
				break; // fall through
		/*
		case RES_CONTENT_FINISHED:
			constructCgi(readBody);
			break;
		case RES_RECV_CGI:
		*/
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

	//ServerManager::registerEvent(pipeSet[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
	cgi->initEnv(m_request);
	cgi->executeCgi(pipeSet, readBody, m_request);
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
