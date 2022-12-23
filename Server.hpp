#ifndef SERVER_HPP
#define SERVER_HPP

// for gnu c portability and c++98 standard. should be removed later
#include <stdint.h>

#include <map>
#include <vector>
#include <string>

#include "Client.hpp"
#include "Location.hpp"
#include "socket_/Socket.hpp"

#define EVENT_SIZE 8

template <typename IoEventPoller>
class	ServerManager;

struct	EventObject
{
	enum	e_type
	{
		EMPTY = 0,
		SERVER = 1,
		CLIENT = 2,
		CGI = 3
	};
	e_type	type;
	void*	object;
};

class	Server
{
public:
// constructors & destructor
	Server();
	~Server();
	Server(Server const& server);
	Server	&operator=(Server const& server);

// member functions
	template <typename IoEventPoller>
	void	initServer();
	template <typename IoEventPoller>
	typename IoEventPoller::EventStatus	handleEvent(const typename IoEventPoller::EventData& event);

private:
	void	setToDefault();

// member variales - config
	std::string					m_index;
	std::string					m_serverNames;
	std::string					m_errorCode;
	std::string					m_root;
	std::string					m_errorPath;
	std::string					m_errorPages;
	sockaddr_in					m_listen;
	int32_t						m_clientMaxBodySize;
	int32_t						m_uriBufferSize;
	std::vector<Location>		m_locationList;

// member variables - socket
	Socket<Tcp>			m_socket;

public:
	const int				m_fd;

	friend class			ServerParser;
	friend class			ServerSocket;
	friend std::ostream&	operator<<(std::ostream& os, const Server& server);

	static const int		s_defaultPort = 8000;
};

template <typename IoEventPoller>
void
Server::initServer()
{
	// TODO: what if fd pool is full?
	if (m_socket.m_fd < 0)
		throw std::runtime_error("server socket() error");
	if (m_socket.bind(&m_listen) < 0)
		throw std::runtime_error("server socket bind() error");
	if (m_socket.listen() < 0)
		throw std::runtime_error("server socket listen() error");
	ServerManager<IoEventPoller>::addEventObject(EventObject::SERVER, m_fd, this);
	ServerManager<IoEventPoller>::registerEvent(m_fd, IoEventPoller::ADD,
			IoEventPoller::READ);
}

template <typename IoEventPoller>
typename IoEventPoller::EventStatus
Server::handleEvent(const typename IoEventPoller::EventData& event)
{
	typename IoEventPoller::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventPoller::READ:
			int		clientFd;
			clientFd = m_socket.accept();

			if (clientFd < 0)
				throw std::runtime_error("accept error in Server::handleEvent()");

			ServerManager<IoEventPoller>::addEventObject(EventObject::CLIENT,
					clientFd, this);
			ServerManager<IoEventPoller>::registerEvent(clientFd,
					IoEventPoller::ADD, filter);
			break;
		default:
			throw std::logic_error("unhandled event filter in Server::handleEvent()");
	}
	return IoEventPoller::NORMAL;
}

#endif
