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
	void	receiveData(int eventInfo);
	void	sendData(int eventInfo);

// member variables
	const Server*		m_server;
	Socket<Tcp>			m_socket;
	Request				m_request;
	Response			m_response;
	std::iostream		m_stream;
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
