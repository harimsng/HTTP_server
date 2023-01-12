#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include "socket_/Socket.hpp"
#include "event/EventObject.hpp"

template <typename IoEventPoller>
class	ServerManager;

class	Server: public IEventObject
{
	typedef	IEventObject::IoEventPoller	IoEventPoller;

// deleted
	Server	&operator=(const Server& server);

public:
// constructors & destructor
	Server(uint16_t port);
	virtual ~Server();
	Server(const Server& server);

// member functions
	void	initServer();

	IoEventPoller::EventStatus	handleEventWork(const IoEventPoller::Event& event);

private:
// member variables - socket
	Socket<Tcp>		m_socket;

public:
	const uint16_t	m_port;
	const int		m_fd;

};

#endif
