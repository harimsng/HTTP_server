#include "communicator/Response.hpp"

using namespace std;

map<int, string> Response::s_statusMessageTable;

Response::Response()
{
}

Response::~Response()
{
}

void
Response::makeResponseHeader(string& buffer)
{
	buffer += "HTTP/1.1 ";
	buffer += getStatusMessage(200);
	buffer += "Content-Length: 0\n";
	buffer += "\r\n";
}

void
Response::setStatusMessageTable(void)
{
	s_statusMessageTable[100] = "Continue";
	s_statusMessageTable[101] = "Switching protocols";
	s_statusMessageTable[200] = "OK";
	s_statusMessageTable[201] = "Created";
	s_statusMessageTable[202] = "Accepted";
	s_statusMessageTable[203] = "Non-authoritative information";
	s_statusMessageTable[204] = "No content";
	s_statusMessageTable[301] = "Moved permanently";
	s_statusMessageTable[302] = "Not temporarily";
	s_statusMessageTable[304] = "Not modified";
	s_statusMessageTable[400] = "Bad Request";
	s_statusMessageTable[401] = "Unauthorized";
	s_statusMessageTable[402] = "Payment required";
	s_statusMessageTable[403] = "Forbidden";
	s_statusMessageTable[404] = "Not found";
	s_statusMessageTable[405] = "Method not allowed";
	s_statusMessageTable[407] = "Proxy authentication required";
	s_statusMessageTable[408] = "Request timeout";
	s_statusMessageTable[410] = "Gone";
	s_statusMessageTable[412] = "Precondition failed";
	s_statusMessageTable[413] = "Payload too large";
	s_statusMessageTable[414] = "Request-URI too long";
	s_statusMessageTable[500] = "Internal server error";
	s_statusMessageTable[501] = "Not implemented";
	s_statusMessageTable[503] = "Service unnailable";
	s_statusMessageTable[504] = "Gateway timeout";
	s_statusMessageTable[505] = "HTTP version not supported";
}

const string
Response::getStatusMessage(int statusCode)
{
	string statusCodeStr;
	map<int, std::string>::iterator statusIt;

	statusIt = s_statusMessageTable.find(statusCode);
	statusCodeStr = std::to_string(statusIt->first) + " ";
	statusCodeStr += statusIt->second + "\n";
	return (statusCodeStr);
}
