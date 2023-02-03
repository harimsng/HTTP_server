#include <fcntl.h>
#include <iostream>

#include "ServerManager.hpp"
#include "io/IoMultiplex.hpp"
#include "event/Cgi.hpp"
#include "http/AutoIndex.hpp"
#include "util/Util.hpp"
#include "responder/GetResponder.hpp"

using namespace std;

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

void
GetResponder::respond()
{
	string readBody;

	switch (m_responseStatus)
	{
		case RES_HEADER:
			respondHeader();
			m_responseStatus = RES_CONTENT; // fall through
		case RES_CONTENT:
			if (m_request.m_file == "")
				readBody = AutoIndex::autoIndex(m_request.m_path, m_request.m_uri);
			else
			{
				if (m_request.m_isCgi == true)
				{
					string tmpFile = m_request.m_path + m_request.m_file + ".temp";
					openFile(tmpFile);
					constructCgi(readBody);
					unlink(tmpFile.c_str());
				}
				else
					readFile(readBody);
			}
			m_sendBuffer.append("Content-Length: ");
			m_sendBuffer.append(Util::toString(readBody.size()));
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.append(g_CRLF);
			//m_sendBuffer.reserve(m_sendBuffer.size() + readBody.size());

			// TODO: change code to use swap instead of appen
			m_sendBuffer.append(readBody);
			m_responseStatus = RES_DONE; // fall through
			// break;
		case RES_DONE:
		// method must know end of response(content length, chunked)
			endResponse();
			break;
		default:
			;
	}
}

void
GetResponder::constructCgi(std::string& readBody)
{
	int	pipeSet[2];

	pipe(pipeSet);
	m_cgiReadEnd = pipeSet[0];

	Cgi*	cgi = new Cgi(m_fileFd, pipeSet[1], m_requestHandler);

	ServerManager::registerEvent(pipeSet[1], Cgi::IoEventPoller::OP_ADD, Cgi::IoEventPoller::FILT_READ, cgi);
	cgi->initEnv(m_request);
	cgi->executeCgi(pipeSet, readBody);
	m_responseStatus = RES_DONE;
}
