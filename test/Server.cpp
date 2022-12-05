#include <sys/socket.h>
#include <netinet/in.h>

#include "Server.hpp"

// constructors & destructor
Server::Server(int listenSocket)
:	m_listenSocket(listenSocket)
{
	init();
}

Server::Server(Server const& server)
:	m_listenSocket(server.m_listenSocket)
{
	init();
	*this = server;
}

Server::~Server()
{
}

void	Server::init()
{
}

// operators
Server&	Server::operator=(Server const& server)
{
	m_connectionVec = server.m_connectionVec;
	return *this;
}

// member functions
int	Server::acceptConnection(fd_set* readSet)
{
	sockaddr_in	socketAddr;
	socklen_t	socketAddrLen;
	int			newConnection;

	if (FD_ISSET(m_listenSocket, readSet) == 0)
	{
		FD_SET(m_listenSocket, readSet);
		return -1;
	}
	newConnection = accept(m_listenSocket, reinterpret_cast<sockaddr*>(&socketAddr), &socketAddrLen);
	m_connectionVec.push_back(std::make_pair(newConnection, Connection(newConnection)));
	return newConnection;
}

int	Server::getConnectionIdx(fd_set* fdSet)
{
	static int	idx = 0;

	for (; idx < m_connectionVec.size(); ++idx)
	{
		int		socketFd = m_connectionVec[idx].first;
		if (FD_ISSET(socketFd, fdSet) != 0)
			return idx;
		FD_SET(socketFd, fdSet);
	}
	idx = 0;
	return m_connectionVec.size();
}

void	Server::readOperation(fd_set* fdSet)
{
	int	connectionIdx = 0;

	while ((connectionIdx = getConnectionIdx(fdSet)) != m_connectionVec.size())
		m_connectionVec[connectionIdx].second.readData();
}

void	Server::writeOperation(fd_set* fdSet)
{
	int	connectionIdx = 0;

	while ((connectionIdx = getConnectionIdx(fdSet)) != m_connectionVec.size())
		m_connectionVec[connectionIdx].second.writeData();
}
