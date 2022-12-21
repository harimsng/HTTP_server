#include <unistd.h>
#include <stdexcept>

#include "Request.hpp"

#define REQUEST_BUFFER_SIZE (8192)

using namespace	std;

// constructors & destructor
Request::Request(const Socket<Tcp>& socket)
:	m_socket(&socket),
	m_buffer(REQUEST_BUFFER_SIZE, 0),
	m_parser(m_buffer)
{
}

Request::~Request()
{
}

Request::Request(const Request& request)
:	m_socket(NULL),
	m_parser(m_buffer)
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

int
Request::receiveRequest(int eventInfo)
{
	int			count;

	count = receiveRawData(eventInfo);
	if (count == 0)
		return 0;
	m_parser.parse(m_headerFieldsMap);
	return count;
}

int
Request::receiveRawData(int eventInfo)
{
	int			count = 0;
	std::size_t	residue = m_buffer.size();

	m_buffer.resize(m_buffer.capacity());
	count = ::read(m_socket->m_fd, const_cast<char*>(m_buffer.data()) + residue,
			m_buffer.size() - residue - 1);
	m_buffer.resize(residue + count + 1, 0);
	(void)eventInfo;
	return count;
}
