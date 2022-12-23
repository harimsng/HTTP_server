#include "Response.hpp"

// constructors & destructor
Response::Response()
{
}

Response::~Response()
{
}

Response::Response(const Response& response)
{
	(void)response;
}

// operators
Response&
Response::operator=(const Response& response)
{
	(void)response;
	return *this;
}

void	Response::sendResponse(int eventInfo)
{
	(void)eventInfo;
}
