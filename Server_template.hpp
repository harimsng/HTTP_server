#ifndef SERVER_HPP
#define SERVER_HPP

// for gnu c portability and c++98 standard. should be removed later
#include <stdint.h>

#include <netinet/in.h>

#include <map>
#include <vector>
#include <string>

#include "Location.hpp"
#include "socket_/Socket.hpp"
#include "communicator/Communicator.hpp"

#define EVENT_SIZE 8

#define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_len*/		INET_ADDRSTRLEN,\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){htonl(addr)},\
/*sin_zero*/	{0, }\
})

template <typename IoEventHandler>
class	Server
{
	typedef typename IoEventHandler::EventData	EventData;
	
public:
// constructors & destructor
	Server();
	~Server();
	Server(Server const& server);
	Server	&operator=(Server const& server);

// member functions
	void	initServer();
	void	handleEvent(const EventData& event);

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
	Socket<TcpSocket>			m_socket;

public:
	const int					m_fd;

	friend class			ServerParser;
	friend class			ServerSocket;
	friend std::ostream&	operator<<(std::ostream& os, const Server& server);

};

template <typename IoEventHandler>
Server<IoEventHandler>::Server()
:	m_fd(m_socket.m_fd)
{
	setToDefault();
}

template <typename IoEventHandler>
Server<IoEventHandler>::~Server()
{
}

template <typename IoEventHandler>
Server<IoEventHandler>::Server(Server const& server)
:	m_socket(server.m_socket),
	m_fd(m_socket.m_fd)
{
	*this = server;
}

template <typename IoEventHandler>
Server<IoEventHandler>&
Server<IoEventHandler>::operator=(const Server& server)
{
	m_index = server.m_index;
	m_serverNames = server.m_serverNames;
	m_errorCode = server.m_errorCode;
	m_root = server.m_root;
	m_errorPages = server.m_errorPages;
	m_listen = server.m_listen;
	m_clientMaxBodySize = server.m_clientMaxBodySize;
	m_uriBufferSize = server.m_uriBufferSize;
	m_locationList = server.m_locationList;
	return *this;
}

template <typename IoEventHandler>
void
Server<IoEventHandler>::setToDefault()
{
//	m_index;
	m_serverNames = "";
//	m_errorCode;
//	m_root;
//	m_errorPages;
	m_listen = GET_SOCKADDR_IN(INADDR_ANY, 8000);
	m_clientMaxBodySize = 1 << 13; // 8kb
//	m_uriBufferSize;
}

template <typename IoEventHandler>
std::ostream&
operator<<(std::ostream& os, const Server<IoEventHandler>& server)
{
	uint32_t	addr = ntohl(server.m_listen.sin_addr.s_addr);

	os << "server\n{\n";
	os << "\tserver_name " << server.m_serverNames << '\n';
	os << "\tlisten " << ((addr & 0xff000000) >> 24) << '.'
	   << ((addr & 0xff0000) >> 16) << '.'
	   << ((addr & 0xff00) >> 8) << '.'
	   << (addr & 0xff) << ':'
	   << ntohs(server.m_listen.sin_port) << '\n';
	os << "\tindex " << server.m_index << '\n';
	os << "\tclient_max_body_size " << server.m_clientMaxBodySize << '\n';
	for (uint32_t i = 0; i < server.m_locationList.size(); ++i)
		os << server.m_locationList[i];
	os << "}\n";
	return os;
}

template <typename IoEventHandler>
void
Server<IoEventHandler>::initServer()
{
	// TODO: what if fd pool is full?
	if (m_socket.bind(&m_listen) < 0)
		throw std::runtime_error("server socket bind() error");
	if (m_socket.listen() < 0)
		throw std::runtime_error("server socket listen() error");
//	ServerManager<IoEventHandler>::s_ioEventHandler.add(m_fd, IoEventHandler::ADD, IoEventHandler::READ);
}

template <typename IoEventHandler>
void
Server<IoEventHandler>::handleEvent(const EventData& event)
{
	typename IoEventHandler::e_filters	filter = event.filter;

	switch (filter)
	{
		case IoEventHandler::READ:
			int	fd;
			fd = m_socket.accept(NULL);

			if (fd < 0)
				throw std::runtime_error("accept error in Server::handleEvent()");
			break;
		default:
			throw std::logic_error("unhandled kevent filter in Server::handleEvent()");
	}
}

#endif
