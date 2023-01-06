#include <unistd.h>

#include "socket/ClientSocket.hpp"

// after use it delete it
#include <iostream>

ClientSocket::ClientSocket()
{
	if (m_response.s_statusMessageTable.size() == 0)
		m_response.setStatusMessageTable();
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
	m_socketAddr = copy.m_socketAddr;
	m_socketAddrSize = copy.m_socketAddrSize;
	m_socketFd = copy.m_socketFd;
	return (*this);
}

void
ClientSocket::createSocket(const sockType& initClass)
{
	int opts;

	m_socketFd = accept(initClass.m_socketFd, (struct sockaddr*)&m_socketAddr,
			&m_socketAddrSize);
	opts = fcntl(m_socketFd, F_SETFL);
	opts = (opts | O_NONBLOCK);
	fcntl(m_socketFd, F_SETFL, opts);
	if (m_socketFd < 0)
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
	read(m_socketFd, &m_requestBuffer[m_request.m_preBufferSize], messageSize);
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
