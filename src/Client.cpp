#include <string>

#include "Client.hpp"

#define BUFFER_SIZE (1 << (10 + 4))

using namespace	std;

// constructors & destructor
Client::Client(int fd)
//	m_httpInfo(server),
:	m_socket(fd),
	m_request(m_socket),
	m_response(m_socket)
//	m_request(m_socket, m_httpInfo),
//	m_response(m_socket, m_httpInfo)
{
}

Client::~Client()
{
}

Client::Client(Client const& client)
//	m_httpInfo(client.m_httpInfo),
:	m_socket(client.m_socket),
	m_request(m_socket),
	m_response(m_socket)
//	m_request(m_socket, m_httpInfo),
//	m_response(m_socket, m_httpInfo)
{
}

Client::IoEventPoller::EventStatus
Client::handleEvent(const IoEventPoller::Event& event)
{
	IoEventPoller::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventPoller::READ:
			Logger::log(Logger::DEBUG, "read event");
			if (m_request.receiveRequest(event.getInfo()) == 0)
				return IoEventPoller::END;
		case IoEventPoller::WRITE:
			// Logger::log(Logger::DEBUG, "write event");
			m_response.sendResponse(event.getInfo());
			return IoEventPoller::NORMAL;
//		case IoEventPoller::EXCEPT:
//			break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
}
// catch (HttpErrorHandler& e)
// {
//     m_response.makeErrorResponse(e.getErrorMessage());
// }

// operators
Client&	Client::operator=(Client const& client)
{
	(void)client;
	throw logic_error("operator=(Client const&) must not be used");
	return *this;
}
