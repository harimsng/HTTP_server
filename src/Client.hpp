#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdexcept>
#include <sstream>
#include <iostream>

//#include "communicator/Request.hpp"
//#include "communicator/Response.hpp"
#include "OsDependency.hpp"
#include "Webserv.hpp"
#include "EventObject.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "socket_/Socket.hpp"

#include "http/HttpInfo.hpp"
#include "Logger.hpp"

class	Server;

class	Client: public IEventObject
{
	typedef	IEventObject::IoEventPoller	IoEventPoller;

// deleted
	Client	&operator=(Client const& client);

public:
// constructors & destructor
	Client(int fd);
	~Client();
	Client(Client const& client);

// member functions
	IoEventPoller::EventStatus	handleEventWork(const IoEventPoller::Event& event);

// member variables
//
// test
//	HttpInfo		m_httpInfo;

	Socket<Tcp>		m_socket;
	Request			m_request;
	Response		m_response;
};

#endif
