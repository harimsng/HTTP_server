#include "communicator/Response.hpp"

using namespace std;

unordered_map<int, string> Response::s_statusCode;

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
	buffer += getStatusCode(200);
	buffer += "Content-Length: 0\n";
	buffer += "\r\n";
}

void
Response::setStatusCode(void)
{
	s_statusCode[100] = "Continue";
	s_statusCode[101] = "Switching protocols";
	s_statusCode[200] = "OK";
	s_statusCode[201] = "Created";
	s_statusCode[202] = "Accepted";
	s_statusCode[203] = "Non-authoritative information";
	s_statusCode[204] = "No content";
	s_statusCode[301] = "Moved permanently";
	s_statusCode[302] = "Not temporarily";
	s_statusCode[304] = "Not modified";
	s_statusCode[400] = "Bad Request";
	s_statusCode[401] = "Unauthorized";
	s_statusCode[402] = "Payment required";
	s_statusCode[403] = "Forbidden";
	s_statusCode[404] = "Not found";
	s_statusCode[405] = "Method not allowed";
	s_statusCode[407] = "Proxy authentication required";
	s_statusCode[408] = "Request timeout";
	s_statusCode[410] = "Gone";
	s_statusCode[412] = "Precondition failed";
	s_statusCode[413] = "Payload too large";
	s_statusCode[414] = "Request-URI too long";
	s_statusCode[500] = "Internal server error";
	s_statusCode[501] = "Not implemented";
	s_statusCode[503] = "Service unnailable";
	s_statusCode[504] = "Gateway timeout";
	s_statusCode[505] = "HTTP version not supported";
}

const string
Response::getDate(const char* format)
{
	time_t		curTime;
	struct tm*	curTimeInfo;
	char		timeBuf[1024];

	time(&curTime);
	curTimeInfo = localtime(&curTime);
	strftime(timeBuf, 1024, format, curTimeInfo);
	return (timeBuf);
}

const string
Response::getStatusCode(int statusCode)
{
	string statusCodeStr;
	unordered_map<int, std::string>::iterator statusIt;

	statusIt = s_statusCode.find(statusCode);
	statusCodeStr = std::to_string(statusIt->first) + " ";
	statusCodeStr += statusIt->second + "\n";
	return (statusCodeStr);
}
