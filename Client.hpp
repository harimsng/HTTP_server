#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdexcept>
#include <sstream>
#include <iostream>

//#include "communicator/Request.hpp"
//#include "communicator/Response.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "socket_/Socket.hpp"

class	Server;

class	Client
{
// deleted
	Client	&operator=(Client const& client);

public:
// constructors & destructor
	Client(Server& server, int fd);
	~Client();
	Client(Client const& client);

// member functions
	template <typename IoEventPoller>
	typename IoEventPoller::EventStatus	handleEvent(const typename IoEventPoller::EventData& event);

// member variables
	const Server*	m_server; // is entire Server information is needed? or root directory is enough?
	Socket<Tcp>		m_socket;
	Request			m_request;
	Response		m_response;
};

template <typename IoEventPoller>
typename IoEventPoller::EventStatus
Client::handleEvent(const typename IoEventPoller::EventData& event)
{
	typename IoEventPoller::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventPoller::READ:
			if (m_request.receiveRequest(event.getInfo()) == 0)
				return IoEventPoller::END;
		case IoEventPoller::WRITE:
			m_response.sendResponse(event.getInfo());
			return IoEventPoller::NORMAL;
//		case IoEventPoller::EXCEPT:
//			break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
}

#endif
