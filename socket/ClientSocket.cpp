#include "ClientSocket.hpp"
#include <unistd.h>

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

// void
// ClientSocket::readSocket(int messageSize)
// {
//     int ret;
//
//     m_requestBuffer.resize(messageSize + m_communicator.m_preBufferSize + 1, 0);
//     read(m_SocketFd, &m_requestBuffer[m_communicator.m_preBufferSize], messageSize);
//
//
//     ret = m_communicator.readRequestHeader(m_requestBuffer);
//     if (ret == Request::REQUEST_HEADER_END)
//         m_communicator.readRequestBody(m_requestBuffer);
// }
//
// void
// ClientSocket::writeSocket()
// {
//     m_communicator.writeSocket(m_SocketFd);
// }

void
ClientSocket::readSocket(int messageSize, Server& server)
{
	(void)server;
	m_requestBuffer.resize(messageSize + m_request.m_preBuffersize + 1, 0);
	read(m_SocketFd, &m_requestBuffer[m_request.m_preBuffersize], messageSize);
	if (m_request.makeRequest(m_requestBuffer) == Request::REQUEST_HEADER_END)
	{

	}
}

void		 ClientSocket::writeSocket()
{

}
