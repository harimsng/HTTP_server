#include <unistd.h>
#include <stdexcept>

#include "Request.hpp"

// for test
#include "Logger.hpp"

#define REQUEST_BUFFER_SIZE (8192)
#define REQUEST_EOF (0)

using namespace	std;

// constructors & destructor
Request::Request(const Socket<Tcp>& socket)
:	m_socket(&socket),
	m_buffer(REQUEST_BUFFER_SIZE, 0),
	m_parser(m_buffer)
{
	m_residue = 0;
}

Request::Request(const Socket<Tcp>& socket, HttpInfo& httpInfo)
:	m_socket(&socket),
	m_buffer(REQUEST_BUFFER_SIZE, 0),
	m_parser(m_buffer),
	m_httpInfo(&httpInfo)
{
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
	m_parser.parse(*this);
	return count;
}

// int
// Request::receiveRawData(int eventInfo)
// {
//     int			count = 0;
//     std::size_t	residue = m_buffer.size();
//
//     (void)eventInfo;
//     // read data from the socket to make zero-terminated buffer.
//     m_buffer.resize(m_buffer.capacity(), 0);
//     count = ::read(m_socket->m_fd, const_cast<char*>(m_buffer.data()) + residue,
//             m_buffer.size() - residue - 1);
//     m_buffer.resize(residue + count + 1, 0);
//     return count;
// }

int
Request::receiveRawData(int eventInfo)
{
	int			count = 0;

	m_buffer.resize(m_buffer.capacity(), 0);
	count = ::read(m_socket->m_fd, const_cast<char*>(m_buffer.data()) + m_residue,
			eventInfo);
	m_buffer.resize(m_residue + count + 1, 0);
	return count;
}

ostream&
operator<<(std::ostream& os, const Request& request)
{
	HeaderFieldsMap::const_iterator mapIt;

	os << "reqeust info\n";
	os << "\tmethod : " << request.m_method << endl;
	os << "\ttarget : " << request.m_target << endl;
	os << "\tprotocol : " << request.m_protocol << endl;
	os << "header field\n";
	for (mapIt = request.m_headerFieldsMap.begin();
			mapIt != request.m_headerFieldsMap.end(); mapIt++)
	{
		os << "\t" << mapIt->first << " : ";
		vector<string>::const_iterator vecIt = mapIt->second.begin();
		for (; vecIt != mapIt->second.end(); vecIt++)
			os << *vecIt << " ";
		os << "\n";
	}
	return (os);
}
