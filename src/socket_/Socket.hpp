#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <unistd.h>
#include <ostream>

#include "SocketTypes.hpp"

template <typename SocketType>
class	Socket
{
// types
	typedef typename SocketType::socketAddr	socketAddr;

// deleted
	Socket	&operator=(const Socket& socket) {(void)socket; return *this;}

public:
// constructors & destructor
	Socket() throw();
	~Socket() throw();
	Socket(const Socket & socket) throw();
	Socket(int fd, const socketAddr* addr = NULL) throw();

// member functions
	int		listen(int backlog = 16) throw();
	int		bind(socketAddr* raddr) throw();
	int		accept(sockaddr* raddr = NULL, socklen_t* sockLen = NULL) const throw();
	int		connect(socketAddr* addr) throw();
	const std::string&	getAddressString() const throw();

// member variables
	const int	m_fd;
private:
	socketAddr	m_addr;
};

template <typename SocketType>
Socket<SocketType>::Socket() throw()
:	m_fd(socket(SocketType::domain, SocketType::type, SocketType::protocol))
{
	int socketOption;

	std::ios_base::Init();
	socketOption = 1;
	setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &socketOption, sizeof(socketOption));

}

template <typename SocketType>
Socket<SocketType>::~Socket() throw()
{
	::close(m_fd);
}

template <typename SocketType>
Socket<SocketType>::Socket(Socket const& socket) throw()
:	m_fd(dup(socket.m_fd))
{
}

template <typename SocketType>
Socket<SocketType>::Socket(int fd, const socketAddr* addr) throw()
:	m_fd(fd)
{
	if (addr != NULL)
		::memcpy(&m_addr, addr, sizeof(*addr));
}

template <typename SocketType>
int
Socket<SocketType>::bind(socketAddr* addr) throw()
{
	::memcpy(&m_addr, addr, sizeof(*addr));
	return ::bind(m_fd, reinterpret_cast<sockaddr*>(addr), SocketType::socketAddrLen);
}

template <typename SocketType>
int
Socket<SocketType>::listen(int backlog) throw()
{
	return ::listen(m_fd, backlog);
}

template <typename SocketType>
int
Socket<SocketType>::accept(sockaddr* raddr, socklen_t* sockLen) const throw()
{
	return ::accept(m_fd, raddr, sockLen);
}

template <typename SocketType>
int
Socket<SocketType>::connect(socketAddr* addr) throw()
{
	return ::connect(m_fd, addr, SocketType::socketAddrLen);
}

// operators
template <typename SocketType>
const std::string&
Socket<SocketType>::getAddressString() const throw()
{
}

#endif
