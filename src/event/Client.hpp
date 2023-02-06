#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdexcept>
#include <sstream>
#include <iostream>

#include "Webserv.hpp"
#include "event/EventObject.hpp"
#include "http/RequestHandler.hpp"
#include "socket/Socket.hpp"

class	Server;

class	Client: public EventObject
{
	typedef	EventObject::IoEventPoller	IoEventPoller;

// deleted
	Client	&operator=(Client const& client);

public:
// constructors & destructor
	Client(int fd);
	virtual ~Client();
	Client(Client const& client);

// member functions
	IoEventPoller::EventStatus	handleEventWork();

// member variables
private:
	Socket<Tcp>		m_socket;
	RequestHandler	m_requestHandler;
};

#endif
