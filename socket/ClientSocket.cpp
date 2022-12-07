#include "ClientSocket.hpp"

ClientSocket::ClientSocket()
{
}


ClientSocket::ClientSocket(const ClientSocket& copy)
{
	*this = copy;
}

ClientSocket::~ClientSocket()
{
}

ClientSocket&
ClientSocket::operator=(const ClientSocket &copy)
{
	m_SocketAddr = copy.m_SocketAddr;
	m_SocketAddrSize = copy.m_SocketAddrSize;
	m_SocketFd = copy.m_SocketFd;
	return (*this);
}

void
ClientSocket::createSocket(const initType& initClass)
{
	int opts;

	m_SocketFd = accept(initClass.m_SocketFd, (struct sockaddr*)&m_SocketAddr,
			&m_SocketAddrSize);
	opts = fcntl(m_SocketFd, F_SETFL);
	opts = (opts | O_NONBLOCK);
	fcntl(m_SocketFd, F_SETFL, opts);
	if (m_SocketFd < 0)
	{
		// throw exception;
	}
}
