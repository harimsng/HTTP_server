#include <unistd.h>
#include <stdexcept>

#include "Request.hpp"

#define REQUEST_BUFFER_SIZE (8192)

using namespace	std;

// constructors & destructor
Request::Request(const Socket<Tcp>& socket)
:	m_socket(&socket),
	m_buffer(REQUEST_BUFFER_SIZE, 0)
{
}

Request::~Request()
{
}

Request::Request(const Request& request)
:	m_socket(NULL)
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
Request::receiveData(int eventInfo)
{
	int			count = 0;
	std::size_t	endPos = 0;

	m_buffer.resize(m_buffer.capacity());
	count = ::read(m_socket->m_fd, const_cast<char*>(m_buffer.data()) + endPos,
			m_buffer.size() - endPos);
	m_buffer.resize(endPos + count + 1, 0);
	if (count == 0)
		return 0;

	endPos = checkBuffer(m_buffer);
	if (endPos == string::npos)
		throw HttpErrorHandler(501);

	m_parser.push(m_buffer);
	m_parser.parse(m_headerFieldsMap);
	(void)eventInfo;
	return count;
}

string::size_type
Request::checkBuffer(std::string& buffer)
{
    switch (m_parser.getReadStatus())
    {
		case HttpRequestParser::STATUS_LINE:
        case HttpRequestParser::HEADER_FIELDS:
            return buffer.find("\r\n");
		case HttpRequestParser::MESSAGE_BODY:
            return buffer.size() - 1;
		case HttpRequestParser::FINISHED:
			throw HttpErrorHandler(413);
        default:
			;
    }
}
