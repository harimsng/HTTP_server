#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <unistd.h>

#include "SocketTypes.hpp"

template <typename SocketType>
class	Socket
{
public:
// types
	typedef typename SocketType::SocketAddr	SocketAddr;

private:
// deleted
	Socket	&operator=(const Socket& socket) {(void)socket; return *this;}

public:
// constructors & destructor
	Socket() throw();
	~Socket() throw();
	Socket(const Socket & socket) throw();
	Socket(int fd, const SocketAddr* addr = NULL) throw();

// member functions
	int		listen(int backlog = 8) throw();
	int		bind(SocketAddr* addr) throw();
	int		accept(sockaddr* raddr = NULL, socklen_t* sockLen = NULL) const throw();
	int		connect(SocketAddr* addr) throw();
	
	const SocketAddr&	getAddress() throw();

// member variables
	const int	m_fd;
	SocketAddr	m_addr;
};

template <typename SocketType>
Socket<SocketType>::Socket() throw()
:	m_fd(socket(SocketType::domain, SocketType::type, SocketType::protocol))
{
	int socketOption = 1;

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
Socket<SocketType>::Socket(int fd, const SocketAddr* addr) throw()
:	m_fd(fd)
{
	if (addr != NULL)
		::memcpy(&m_addr, addr, sizeof(*addr));
}

template <typename SocketType>
int
Socket<SocketType>::bind(SocketAddr* addr) throw()
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
Socket<SocketType>::connect(SocketAddr* addr) throw()
{
	return ::connect(m_fd, addr, SocketType::socketAddrLen);
}

template <typename SocketType>
const typename Socket<SocketType>::SocketAddr&
Socket<SocketType>::getAddress() throw()
{
	SocketAddr	addr;
	socklen_t	len;

	getsockname(m_fd, reinterpret_cast<sockaddr*>(&addr), &len);
	m_addr = addr;
	return m_addr;
}

#endif
