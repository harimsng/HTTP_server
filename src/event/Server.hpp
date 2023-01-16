#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include "socket_/Socket.hpp"
#include "event/EventObject.hpp"

class	ServerManager;

class	Server: public EventObject
{
	typedef	EventObject::IoEventPoller	IoEventPoller;

// deleted
	Server	&operator=(const Server& server);

public:
// constructors & destructor
	Server(uint16_t port);
	virtual ~Server();
	Server(const Server& server);

// member functions
	void	initServer();

	IoEventPoller::EventStatus	handleEventWork();

private:
// member variables - socket
	Socket<Tcp>		m_socket;

public:
	const uint16_t	m_port;
};

#endif
