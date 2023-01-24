#include <string>

#include "Client.hpp"

using namespace	std;

// constructors & destructor
Client::Client(int fd)
:	EventObject(fd),
	m_socket(fd),
	m_requestHandler(m_socket)
{
}

Client::~Client()
{
}

Client::Client(Client const& client)
:	EventObject(client),
	m_socket(client.m_socket),
	m_requestHandler(m_socket)
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
			if (m_requestHandler.receiveRequest() == RequestHandler::RECV_END)
				return IoEventPoller::END;
			break;
		case IoEventPoller::WRITE:
			LOG(DEBUG, "write event to client");
			m_requestHandler.sendResponse();
			break;
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
