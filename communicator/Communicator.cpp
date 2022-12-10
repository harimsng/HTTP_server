#include "Communicator.hpp"
#include "Request.hpp"

using namespace std;

Communicator::Communicator(int socketFd)
{
	m_socketFd = socketFd;
	m_preBufferSize = 0;
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
Communicator::readSocket(int messageSize) try
{
	int 	readRet;

	m_stringBuffer.resize(messageSize + m_preBufferSize, 0);
	readRet = read(m_socketFd, &m_stringBuffer[m_preBufferSize], messageSize - 1);
	if (m_request.getRequestSection() == Request::REQUEST_LINE)
		m_stringBuffer.erase(0, m_request.makeRequestLine(m_stringBuffer));
	if (m_request.getRequestSection() == Request::REQUEST_HEADER)
		m_stringBuffer.erase(0, m_request.makeRequestHeader(m_stringBuffer, m_requestHeader));
	if (m_request.getRequestSection() == Request::REQUEST_BODY)
		m_request.makeReqeustBody(m_stringBuffer);
	m_preBufferSize = m_stringBuffer.size() - 1;
}
catch(exception& e)
{

}

void
Communicator::writeSocket()
{

}
