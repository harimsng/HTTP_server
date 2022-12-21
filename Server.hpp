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
#include "communicator/Communicator.hpp"

#define EVENT_SIZE 8

template <typename IoEventHandler>
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
	void*	target;
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
	template <typename IoEventHandler>
	void	initServer();
	template <typename IoEventHandler>
	void	handleEvent(const typename IoEventHandler::EventData& event);

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

template <typename IoEventHandler>
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
	ServerManager<IoEventHandler>::addEventTarget(EventObject::SERVER, m_fd, this);
	ServerManager<IoEventHandler>::registerEvent(m_fd, IoEventHandler::ADD,
			IoEventHandler::READ);
}

template <typename IoEventHandler>
void
Server::handleEvent(const typename IoEventHandler::EventData& event)
{
	typename IoEventHandler::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventHandler::READ:
			int		clientFd;
			clientFd = m_socket.accept();

			if (clientFd < 0)
				throw std::runtime_error("accept error in Server::handleEvent()");

			ServerManager<IoEventHandler>::addEventTarget(EventObject::CLIENT,
					clientFd, this);
			ServerManager<IoEventHandler>::registerEvent(clientFd,
					IoEventHandler::ADD, filter);
			break;
		default:
			throw std::logic_error("unhandled event filter in Server::handleEvent()");
	}
}

#endif
