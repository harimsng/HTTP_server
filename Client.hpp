#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdexcept>
#include <sstream>
#include <iostream>

#include "communicator/Request.hpp"
#include "communicator/Response.hpp"
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
	template <typename IoEventHandler>
	void	handleEvent(const typename IoEventHandler::EventData& event);

// member variables
	const Server*	m_server; // is entire Server information is needed? or root directory is enough?
	Socket<Tcp>		m_socket;
	Request			m_request;
	Response		m_response;
};

template <typename IoEventHandler>
void
Client::handleEvent(const typename IoEventHandler::EventData& event)
{
	typename IoEventHandler::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventHandler::READ:
			m_request.receiveData(event.getInfo());
			break;
		case IoEventHandler::WRITE:
			m_response.sendData(event.getInfo());
			break;
//		case IoEventHandler::EXCEPT:
//			break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
}

#endif
