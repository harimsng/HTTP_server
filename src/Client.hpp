#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdexcept>
#include <sstream>
#include <iostream>

//#include "communicator/Request.hpp"
//#include "communicator/Response.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "socket_/Socket.hpp"

#include "http/HttpInfo.hpp"
// for test
#include "Logger.hpp"

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
	template <typename IoEventPoller>
	typename IoEventPoller::EventStatus	handleEvent(const typename IoEventPoller::EventData& event);

// member variables
	const ServerNameTable*	m_serverNameTable; // is entire Server information is needed? or root directory is enough?

	// test
	HttpInfo		m_httpInfo;

	Socket<Tcp>		m_socket;
	Request			m_request;
	Response		m_response;

};

template <typename IoEventPoller>
typename IoEventPoller::EventStatus
Client::handleEvent(const typename IoEventPoller::EventData& event)
{
	typename IoEventPoller::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventPoller::READ:
			Logger::log(Logger::DEBUG, "read event");
			if (m_request.receiveRequest(event.getInfo()) == 0)
				return IoEventPoller::END;
			break;
		case IoEventPoller::WRITE:
			Logger::log(Logger::DEBUG, "write event");
			m_response.sendResponse(event.getInfo());
			return IoEventPoller::NORMAL;
			break;
//		case IoEventPoller::EXCEPT:
			// break;
		default:
			throw std::runtime_error("not handled event filter in Client::handleEvent()");
	}
	return (IoEventPoller::END);
}
// catch (HttpErrorHandler& e)
// {
//     m_response.makeErrorResponse(e.getErrorMessage());
// }

#endif
