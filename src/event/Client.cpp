#include <string>

#include "Logger.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "ServerManager.hpp"
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
	static unsigned short	count = 0;

	LOG(INFO, "[%5hu][%5d] client connection closed", count++, m_fd);
}

Client::Client(Client const& client)
:	EventObject(client),
	m_socket(client.m_socket),
	m_requestHandler(m_socket)
{
	m_fd = client.m_fd;
}

// how poller distinguish events
// kqueue: fd, filter, filter options
// epoll: fd
Client::IoEventPoller::EventStatus
Client::handleReadEventWork()
{
	int	status;

	// there are fd leaks. if second tester execution on running webserv.
	status = m_requestHandler.receiveRequest();
	switch (status)
	{
		case RequestHandler::RECV_EVENT:
			LOG(DEBUG, "[%d] registering write event", m_socket.m_fd);
			ServerManager::registerEvent(m_socket.m_fd, IoEventPoller::OP_MODIFY,
					IoEventPoller::FILT_READ | IoEventPoller::FILT_WRITE, this);
			break;

		case RequestHandler::RECV_ERROR:
			if (m_eventStatus == EVENT_EOF)
			{
				return IoEventPoller::STAT_END;
			}
			break;

		default:
			;
	}
	return IoEventPoller::STAT_NORMAL;
}

// NOTE: connection: closed issue?
Client::IoEventPoller::EventStatus
Client::handleWriteEventWork()
{
	int	status;

	status = m_requestHandler.sendResponse();
	switch (status)
	{
		case RequestHandler::SEND_END:
			// this can delete fd instead of modifying 
			LOG(DEBUG, "[%d] write event finished", m_socket.m_fd);
			ServerManager::registerEvent(m_socket.m_fd, IoEventPoller::OP_DELETE,
					IoEventPoller::FILT_WRITE, this);
			break;

		case RequestHandler::SEND_ERROR:
			LOG(DEBUG, "[%d] error while write event", m_socket.m_fd);
			return IoEventPoller::STAT_END;

		default:
			;
	}
	return IoEventPoller::STAT_NORMAL;
}

Client::IoEventPoller::EventStatus
Client::handleErrorEventWork()
{
	if (m_eventStatus == EVENT_EOF)
		return IoEventPoller::STAT_END;
	return IoEventPoller::STAT_ERROR;
}

// operators
Client&	Client::operator=(Client const& client)
{
	(void)client;
	throw logic_error("operator=(Client const&) must not be used");
	return *this;
}
