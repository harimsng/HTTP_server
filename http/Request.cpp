#include <stdexcept>

#include "Request.hpp"

// constructors & destructor
Request::Request()
{
}

Request::~Request()
{
}

Request::Request(const Request& request)
{
	(void)request;
}

// operators
Request&
Request::operator=(const Request& request)
{
	(void)request;
	return *this;
}

void
Request::readRequest(std::string& buffer)
{
	if (m_parser.checkStream(buffer) == false)
		return;
	m_parser.parse(m_headerFieldsMap);
}
