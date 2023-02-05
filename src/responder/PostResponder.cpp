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
	std::string tmpFile = m_request.m_path + m_request.m_file + ".temp";
	struct stat st;
	int status = 0;

	switch (m_responseStatus)
	{
		case RES_HEADER:
			respondHeader(); // fall through
			openFile(tmpFile);
		case RES_CONTENT:
			if (m_request.m_headerFieldsMap.count("CONTENT-LENGTH") > 0)
				status = normalReadBody();
			else
				status = chunkedReadBody();
			if (status)
				m_responseStatus = RES_CONTENT_FINISHED;
			else
				break; // fall through
		case RES_CONTENT_FINISHED:
			if (m_request.m_isCgi == true)
			{
				lseek(m_fileFd, 0, SEEK_SET);
				fstat(m_fileFd, &st);
				m_request.m_bodySize = st.st_size;
				m_request.requestBodyBuf.resize(m_request.m_bodySize , 0);
				read(m_fileFd, (char *)(m_request.requestBodyBuf.data()), m_request.m_bodySize);
				openFile(tmpFile);
				constructCgi(readBody);
				unlink(tmpFile.c_str());
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
		/*
		case RES_RECV_CGI:
		*/
		case RES_DONE:
			endResponse();
		default:
			;
	}
}
catch(int statusCode)
{
	m_request.m_status = statusCode;
	m_sendBuffer.clear();
	m_sendBuffer.append(g_httpVersion);
	m_sendBuffer.append(" ");
	m_sendBuffer.append(Util::toString(statusCode));
	m_sendBuffer.append(" ");
	m_sendBuffer.append(HttpErrorHandler::getErrorMessage(statusCode));
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append("Content-Type: text/html");
	m_sendBuffer.append(g_CRLF);
	m_sendBuffer.append(g_CRLF);
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
	m_responseStatus = RES_RECV_CGI;
}
