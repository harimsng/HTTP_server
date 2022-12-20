#include <stdexcept>

#include "Request.hpp"

// constructors & destructor
Request::Request()
{
	m_stream.basic_ios::rdbuf(const_cast<char*>(m_buffer.data()));
}

Request::~Request()
{
	awf
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
Request::receiveData(int fd, int eventInfo)
{
	
}

/*
	if (m_parser.checkStream(buffer) == false)
		return;
	m_parser.parse(m_headerFieldsMap);
	*/
