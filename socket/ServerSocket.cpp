#include "ServerSocket.hpp"
#include "Server.hpp"

ServerSocket::ServerSocket()
{
}

ServerSocket::~ServerSocket()
{
}

void
ServerSocket::createSocket(const initType& initClass)
{
	setSocketFd(AF_INET, SOCK_STREAM, 0);
	// m_SocketAddr = initClass;
	initAddr(initClass);
	bindSocket();
	listenSocket();
}

void
ServerSocket::setSocketFd(int domain, int type, int protocol)
{
	int option;

	option = 1;
	m_SocketFd = socket(domain, type, protocol);
	fcntl(m_SocketFd, F_SETFL, O_NONBLOCK);
	if (m_SocketFd == -1)
	{
		// throw exception("create socket fail");
	}
	setsockopt(m_SocketFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

}

void
ServerSocket::initAddr(const initType& initClass)
{
	std::memset(&m_SocketAddr, 0, sizeof(m_SocketAddr));
	m_SocketAddr.sin_family = initClass.sin_family;
	m_SocketAddr.sin_port = initClass.sin_port;
	m_SocketAddr.sin_addr.s_addr = INADDR_ANY;
}

void
ServerSocket::bindSocket()
{
	int bindRet;

	bindRet = bind(m_SocketFd, (const struct sockaddr*)&m_SocketAddr, sizeof(m_SocketAddr));
	if (bindRet == -1)
	{
		// throw exception;
	}
}

void
ServerSocket::listenSocket()
{
	int listenRet;

	listenRet = listen(m_SocketFd, BACKLOG);
	if (listenRet == -1)
	{
		// throw exception;
	}
}
