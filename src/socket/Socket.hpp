#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>

template <typename SocketType>
class	Socket
{
// deleted
	Socket	&operator=(const Socket& socket) {(void)socket; return *this;}

public:
// types
	typedef typename SocketType::SocketAddr	SocketAddr;

// constructors & destructor
	Socket();
	~Socket();
	Socket(const Socket& socket);
	Socket(int fd);
	// const Socket&	operator=(const Socket& socket) = default;

private:
	void	init();

public:
// member functions
	int		listen(int backlog = 10000);
	int		bind(SocketAddr* addr);
	int		accept(sockaddr* raddr = 0, socklen_t* sockLen = 0) const;
	int		connect(SocketAddr* addr);

	int		setsockopt(int level, int opt_name, const void* opt_value, socklen_t opt_len);

	const SocketAddr&	getAddress() const;
	void				setAddress();

// member variables
	const int	m_fd;
	SocketAddr	m_addr;
};

template <typename SocketType>
Socket<SocketType>::Socket()
:	m_fd(socket(SocketType::domain, SocketType::type, SocketType::protocol))
{
	init();

}

template <typename SocketType>
Socket<SocketType>::Socket(int fd)
:	m_fd(fd)
{
	init();
	setAddress();
}

template <typename SocketType>
Socket<SocketType>::~Socket()
{
	SocketType::removeSocket(m_fd, m_addr);
}

template <typename SocketType>
Socket<SocketType>::Socket(Socket const& socket)
:	m_fd(dup(socket.m_fd))
{
	*this = socket;
}

template <typename SocketType>
void
Socket<SocketType>::init()
{
	SocketType::initSocket(m_fd);
}

template <typename SocketType>
int
Socket<SocketType>::bind(SocketAddr* addr)
{
	int	ret = ::bind(m_fd, reinterpret_cast<sockaddr*>(addr), SocketType::socketAddrLen);
	if (ret == 0)
		m_addr = *addr;
	return ret;
}

template <typename SocketType>
int
Socket<SocketType>::listen(int backlog)
{
	return ::listen(m_fd, backlog);
}

template <typename SocketType>
int
Socket<SocketType>::accept(sockaddr* raddr, socklen_t* sockLen) const
{
	return ::accept(m_fd, raddr, sockLen);
}

template <typename SocketType>
int
Socket<SocketType>::connect(SocketAddr* addr)
{
	return ::connect(m_fd, addr, SocketType::socketAddrLen);
}

template <typename SocketType>
int	
Socket<SocketType>::setsockopt(int level, int opt_name, const void* opt_value, socklen_t opt_len)
{
	return ::setsockopt(m_fd, level, opt_name, opt_value, opt_len);
}

template <typename SocketType>
const typename Socket<SocketType>::SocketAddr&
Socket<SocketType>::getAddress() const
{
	return m_addr;
}

template <typename SocketType>
void
Socket<SocketType>::setAddress()
{
	socklen_t	len = SocketType::socketAddrLen;

	getsockname(m_fd, reinterpret_cast<sockaddr*>(&m_addr), &len);
}

#endif
