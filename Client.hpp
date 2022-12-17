#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdexcept>

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
	const Server*		m_server;
	Socket<TcpSocket>	m_socket;
};

template <typename IoEventHandler>
void
Client::handleEvent(const typename IoEventHandler::EventData& event)
{
	typename IoEventHandler::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventHandler::READ:
			read
			break;
		case IoEventHandler::WRITE:
			break;
		case IoEventHandler::EXCEPT:
			break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
}

#endif
