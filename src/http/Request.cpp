#include <unistd.h>
#include <stdexcept>

// for test
#include "Logger.hpp"
#include "Request.hpp"
#include "exception/HttpErrorHandler.hpp"

#define REQUEST_BUFFER_SIZE (8192)
#define REQUEST_EOF (0)

using namespace	std;

// constructors & destructor
Request::Request(const Socket<Tcp>& socket)
:	m_socket(&socket),
	m_buffer(REQUEST_BUFFER_SIZE + (REQUEST_BUFFER_SIZE >> 8), 0),
	m_parser(m_buffer)
{
	m_buffer.clear();
	m_residue = 0;
}

Request::Request(const Socket<Tcp>& socket, HttpInfo& httpInfo)
:	m_socket(&socket),
	m_buffer(REQUEST_BUFFER_SIZE + (REQUEST_BUFFER_SIZE >> 8), 0),
	m_parser(m_buffer),
	m_httpInfo(&httpInfo)
{
	m_buffer.clear();
	m_residue = 0;
}

Request::~Request()
{
}

Request::Request(const Request& request)
:	m_socket(NULL),
	m_parser(m_buffer),
	m_httpInfo(NULL)
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
		return REQUEST_EOF;
	else if (count == -1)
		throw HttpErrorHandler(500);
	m_parser.parse(*this);
	return count;
}

int
Request::receiveRawData(int eventInfo)
{
	const int	residue = m_buffer.size();
	int			count = 0;

	(void)eventInfo;
#ifdef __APPLE__
	m_buffer.resize(REQUEST_BUFFER_SIZE, 0);
	count = ::read(m_socket->m_fd, const_cast<char*>(m_buffer.data()) + residue,
			m_buffer.size() - residue - 1);
	// if evenInfo + residue is bigger than buffer size read() will make buffer overflow.
	m_buffer.resize(m_residue + count + 1, 0);
#elif __linux__
	m_buffer.resize(REQUEST_BUFFER_SIZE, 0);
	count = ::read(m_socket->m_fd, const_cast<char*>(m_buffer.data()) + residue,
			m_buffer.size() - residue - 1);
	m_buffer.resize(residue + count + 1, 0);
#endif
	return count;
}
