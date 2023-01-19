#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include "socket/Socket.hpp"
#include "event/EventObject.hpp"

class	ServerManager;

class	Server: public EventObject
{
	typedef	EventObject::IoEventPoller	IoEventPoller;

// deleted
	Server	&operator=(const Server& server);

public:
// constructors & destructor
	Server();
	virtual ~Server();
	Server(const Server& server);

// member functions
	void	initServer(uint32_t addr, uint16_t port);

	IoEventPoller::EventStatus	handleEventWork();

private:
// member variables - socket
	Socket<Tcp>		m_socket;
};

#endif
