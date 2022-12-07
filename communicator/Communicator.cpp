#include "Communicator.hpp"
#include "Request.hpp"

using namespace std;

Communicator::Communicator(int socketFd)
{
	m_socketFd = socketFd;
}

Communicator::~Communicator()
{
}

Communicator::Communicator(const Communicator& copy)
{
	*this = copy;
}

Communicator&
Communicator::operator=(const Communicator &copy)
{
	m_socketFd = copy.m_socketFd;
	return (*this);
}

void
Communicator::readSocket(int messageSize)
{
	int 	readRet;

	m_stringBuffer.resize(messageSize, 0);
	readRet = read(m_socketFd, &m_stringBuffer[0], messageSize - 1);
	if (m_request.getRequestSection() == Request::REQUEST_LINE)
		m_stringBuffer.erase(0, m_request.makeRequestLine(m_stringBuffer));
	if (m_request.getRequestSection() == Request::REQUEST_HEADER)
		m_request.makeRequestHeader(m_stringBuffer, m_requestHeader);
	if (m_request.getRequestSection() == Request::REQUEST_BODY)
		m_request.makeReqeustBody(m_stringBuffer);
}

void
Communicator::writeSocket()
{

}
