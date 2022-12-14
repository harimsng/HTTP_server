#include "Server.hpp"
#include "socket/ServerSocket.hpp"

#define BACKLOG 256

ServerSocket::ServerSocket()
{
}

ServerSocket::~ServerSocket()
{
}

void
ServerSocket::createSocket(const sockaddr_in& sockaddrInet)
{
	setSocketFd(AF_INET, SOCK_STREAM, 0);
	// m_SocketAddr = sockaddrInet;
	initAddr(sockaddrInet);
	bindSocket();
	listenSocket();
}

void
ServerSocket::setSocketFd(int domain, int type, int protocol)
{
	int option;

	option = 1;
	m_socketFd = socket(domain, type, protocol);
	fcntl(m_socketFd, F_SETFL, O_NONBLOCK);
	if (m_socketFd == -1)
	{
		// throw exception("create socket fail");
	}
	setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

}

void
ServerSocket::initAddr(const sockaddr_in& sockaddrInet)
{
	std::memset(&m_socketAddr, 0, sizeof(m_socketAddr));
	m_socketAddr.sin_family = sockaddrInet.sin_family;
	m_socketAddr.sin_port = sockaddrInet.sin_port;
	m_socketAddr.sin_addr.s_addr = INADDR_ANY;
	m_socketAddrSize = INET_ADDRSTRLEN;
}

void
ServerSocket::bindSocket()
{
	int bindRet;

	bindRet = bind(m_socketFd,
				   reinterpret_cast<const struct sockaddr*>(&m_socketAddr),
				   m_socketAddrSize);
	if (bindRet == -1)
	{
		// throw exception;
	}
}

void
ServerSocket::listenSocket()
{
	int listenRet;

	listenRet = listen(m_socketFd, BACKLOG);
	if (listenRet == -1)
	{
		// throw exception;
	}
}
