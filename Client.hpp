#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdexcept>

#include "communicator/Request.hpp"
#include "communicator/Response.hpp"
#include "socket_/Socket.hpp"

class	Server;

class	Client
{
// deleted
	Client	&operator=(Client const& client);
	Client(Client const& client);

public:
// constructors & destructor
	Client(Server& server, int fd);
	~Client();

	template <typename IoEventHandler>
	void	handleEvent(const typename IoEventHandler::EventData& event);

// member functions

// member variables
	Request			m_request;
	Response		m_response;
	const Server*	m_server;
	Socket<Tcp>		m_socket;
};

template <typename IoEventHandler>
void
Client::handleEvent(const typename IoEventHandler::EventData& event)
{
	typename IoEventHandler::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventHandler::READ:
			receiveData(event.getInfo());
			break;
		case IoEventHandler::WRITE:
			sendData(event.getInfo());
			break;
//		case IoEventHandler::EXCEPT:
//			break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
}

#endif
