#include <string>

#include "Client.hpp"

using namespace	std;

// constructors & destructor
Client::Client(int fd)
:	m_socket(fd),
	m_httpInfo(),
	// m_request(m_socket),
	// m_response(m_socket)
	m_request(m_socket, m_httpInfo),
	m_response(m_request)
{
	m_fd = fd;
}

Client::~Client()
{
}

Client::Client(Client const& client)
:	m_socket(client.m_socket),
	m_httpInfo(client.m_httpInfo),
	// m_request(m_socket),
	// m_response(m_socket)
	m_request(m_socket, m_httpInfo),
	m_response(m_request)
{
	m_fd = client.m_fd;
}

Client::IoEventPoller::EventStatus
Client::handleEventWork()
{
	switch (m_filter)
	{
		case IoEventPoller::READ:
			LOG(DEBUG, "read event to client");
			if (m_request.receiveRequest() == 0)
				return IoEventPoller::END;
			m_response.makeResponse();
			break;
		case IoEventPoller::WRITE:
			LOG(DEBUG, "write event to client");
			m_response.sendResponse();
			return IoEventPoller::NORMAL;
//		case IoEventPoller::EXCEPT:
//			break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
	return (IoEventPoller::NORMAL);
}
// catch (HttpErrorHandler& e)
// {
//     m_response.makeErrorResponse(e.getErrorMessage());
//     return (IoEventPoller::NORMAL);
// }

// operators
Client&	Client::operator=(Client const& client)
{
	(void)client;
	throw logic_error("operator=(Client const&) must not be used");
	return *this;
}
