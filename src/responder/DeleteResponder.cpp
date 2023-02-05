#include "DeleteResponder.hpp"
#include "util/Util.hpp"

using namespace std;

// constructors & destructor
DeleteResponder::DeleteResponder(RequestHandler& requestHandler)
:	AResponder(requestHandler)
{
}

DeleteResponder::~DeleteResponder()
{
}

// operators
DeleteResponder&
DeleteResponder::operator=(const DeleteResponder& deleteMethod)
{
	(void)deleteMethod;
	return *this;
}

void
DeleteResponder::deleteFile(const string& filePath, string& readBody)
{
	int		deleteStatus;
	string	responseText = "File is deleted.";

	deleteStatus = unlink(filePath.c_str());
	switch (errno)
	{
		case EACCES:
			responseText = "Permission denied.";
			break;
		case EBUSY:
			responseText = "This file currently being used.";
			break;
		case ENOENT:
			responseText = "File does not exist.";
			break;
		case ENOTDIR:
			responseText = "Path contains none directory.";
			break;
		case EPERM:
			responseText = "Path is a directory";
			break;
		default:
			responseText = "File is deleted.";
			break;
	}
	readBody =
	"<!DOCTYPE html>\n"
	"<html>\n"
	"	<head>\n"
	"		<title>DELETE response</title>\n"
	"	</head>\n"
	"	<body>\n"
	"		<h1>"
	+ responseText +
	"		</h1>\n"
	"	</body>\n"
	"</html>\n";
}

void
DeleteResponder::respond()
{
	string	readBody;
	string	filePath = m_request.m_path + m_request.m_file;

	switch (m_responseStatus)
	{
		case RES_HEADER:
			respondHeader();
			m_responseStatus = RES_CONTENT; // fall through
		case RES_CONTENT:
			deleteFile(filePath, readBody);
			m_sendBuffer.append("Content-Length: ");
			m_sendBuffer.append(Util::toString(readBody.size()));
			m_sendBuffer.append(g_CRLF);
			m_sendBuffer.append(g_CRLF);
			//m_sendBuffer.reserve(m_sendBuffer.size() + readBody.size());

			// TODO: change code to use swap instead of append
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
	/*
	int deleteStatus;

	string filePath = m_request.m_path + m_request.m_file;

	readFile(m_sendBuffer); // 파일 읽기
	deleteStatus = unlink(filePath.c_str());
	if (deleteStatus == -1)
		m_statusCode = 202;
	*/
}
