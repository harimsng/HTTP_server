#include <cerrno>

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
	// NOTE: referencing errno when there's no error is UB
	if (deleteStatus < 0)
	{
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
			// NOTE: there're other errors except those above. they will directed here.
			default:
				responseText = "unlink error.";
				break;
		}
	}
	else
		responseText = "File is deleted.";
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
			m_sendBuffer.append(readBody);
			m_responseStatus = RES_DONE; // fall through
		case RES_DONE:
			endResponse();
			break;
		default:
			;
	}
}
