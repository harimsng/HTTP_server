#include "ClientSocket.hpp"
#include <unistd.h>

// after use it delete it
#include <iostream>

ClientSocket::ClientSocket()
{
	m_response.setStatusCode();
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

void
ClientSocket::readSocket(int messageSize, Server& server)
{
	(void)server;
	int& requestSection = m_request.getRequestSection();

	m_requestBuffer.resize(messageSize + m_request.m_preBufferSize + 1, 0);
	read(m_SocketFd, &m_requestBuffer[m_request.m_preBufferSize], messageSize);
	if (requestSection < Request::REQUEST_HEADER_END)
		m_request.makeRequest(m_requestBuffer);
	if (requestSection == Request::REQUEST_HEADER_END)
	{
		// checkUri(server);
		m_response.makeResponseHeader(m_responseBuffer);
		requestSection = Request::REQUEST_BODY;
	}
	if (requestSection == Request::REQUEST_BODY)
	{
		// m_request.makeRequest(m_requestBuffer);
	}
}

void
ClientSocket::writeSocket()
{
	int writeRet;

	writeRet = write(m_SocketFd, &m_responseBuffer[0], m_responseBuffer.size());
	m_responseBuffer.erase(0, writeRet);
}

void
ClientSocket::checkUri(Server& server)
{
	string uri;
	(void)server;

	uri = m_request.m_uri;
}
