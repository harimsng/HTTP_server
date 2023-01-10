#ifndef SERVER_HPP
#define SERVER_HPP

#include "Webserv.hpp"
#include "socket_/Socket.hpp"
#include "EventObject.hpp"

template <typename IoEventPoller>
class	ServerManager;

class	Server: public IEventObject
{
	typedef	IEventObject::IoEventPoller	IoEventPoller;

// deleted
	Server	&operator=(const Server& server);

public:
// constructors & destructor
	Server();
	virtual ~Server();
	Server(const Server& server);

// member functions
	void	initServer();

	IoEventPoller::EventStatus	handleEvent(const IoEventPoller::Event& event);

private:
// member variables - socket
	Socket<Tcp>			m_socket;
	uint16_t			m_port;

public:
	const int	m_fd;

};

#endif
