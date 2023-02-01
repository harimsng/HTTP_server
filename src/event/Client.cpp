#include <string>

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
	int status;

	switch (m_filter)
	{
		case IoEventPoller::READ:
			LOG(DEBUG, "read event to client(fd=%d)", m_socket.m_fd);
			status = m_requestHandler.receiveRequest();
			switch (status)
			{
				case RequestHandler::RECV_END:
					return IoEventPoller::END;

				case RequestHandler::RECV_EVENT:
					LOG(DEBUG, "registering write event for fd=%d\n", m_socket.m_fd);
					ServerManager::registerEvent(m_socket.m_fd, IoEventPoller::MODIFY,
							IoEventPoller::WRITE, this);
					break;

				default:
					;
			}
			break;

		case IoEventPoller::WRITE:
			LOG(DEBUG, "write event to client(fd=%d)", m_socket.m_fd);
			status = m_requestHandler.sendResponse();
			switch (status)
			{
				case RequestHandler::SEND_DONE:
					LOG(DEBUG, "deleting write event for fd=%d\n", m_socket.m_fd);
					ServerManager::registerEvent(m_socket.m_fd, IoEventPoller::DELETE,
							IoEventPoller::WRITE, this);
					break;

				default:
					;
			}
			break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
	return (IoEventPoller::NORMAL);
}

// operators
Client&	Client::operator=(Client const& client)
{
	(void)client;
	throw logic_error("operator=(Client const&) must not be used");
	return *this;
}
