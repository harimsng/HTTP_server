#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <unistd.h>

#include "SocketTypes.hpp"

template <typename SocketType>
class	Socket
{
// deleted
	Socket	&operator=(const Socket& socket) {(void)socket; return *this;}

public:
// types
	typedef typename SocketType::SocketAddr	SocketAddr;

// constructors & destructor
	Socket() throw();
	~Socket() throw();
	Socket(const Socket & socket) throw();
	Socket(int fd) throw();

private:
	void	init() throw();

public:
// member functions
	int		listen(int backlog = 8) throw();
	int		bind(SocketAddr* addr) throw();
	int		accept(sockaddr* raddr = NULL, socklen_t* sockLen = NULL) const throw();
	int		connect(SocketAddr* addr) throw();
	
	SocketAddr	getAddress() const throw();

// member variables
	const int	m_fd;
};

template <typename SocketType>
Socket<SocketType>::Socket() throw()
:	m_fd(socket(SocketType::domain, SocketType::type, SocketType::protocol))
{
	init();

}

template <typename SocketType>
Socket<SocketType>::Socket(int fd) throw()
:	m_fd(fd)
{
	init();
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
void
Socket<SocketType>::init() throw()
{
	int socketOption = 1;

	setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &socketOption, sizeof(socketOption));
}

template <typename SocketType>
int
Socket<SocketType>::bind(SocketAddr* addr) throw()
{
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
typename Socket<SocketType>::SocketAddr
Socket<SocketType>::getAddress() const throw()
{
	SocketAddr	addr;
	socklen_t	len = Tcp::socketAddrLen;

	getsockname(m_fd, reinterpret_cast<sockaddr*>(&addr), &len);
	return addr;
}

#endif
