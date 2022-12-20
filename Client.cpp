#include <string>

#include "Client.hpp"

#define BUFFER_SIZE (1 << (10 + 4))

using namespace	std;

// constructors & destructor
Client::Client(Server& server, int fd)
:	m_server(&server),
	m_socket(fd)
{
}

Client::~Client()
{
}

Client::Client(Client const& client)
:	//Client(client.m_server, client.m_socket), delegating constructor C++11 feature
	m_server(client.m_server),
	m_socket(client.m_socket)
{
}

// operators
Client&	Client::operator=(Client const& client)
{
	(void)client;
	throw logic_error("operator=(Client const&) must not be used");
	return *this;
}

void
Client::receiveData(int eventInfo)
{
	size_t	residue;

	(void)eventInfo;
	residue = m_receiveBuffer.size();
	m_receiveBuffer.resize(m_receiveBuffer.capacity());
	read(m_socket.m_fd, &m_receiveBuffer[0] + residue, m_receiveBuffer.size() - residue);
	m_request.readRequest(m_receiveBuffer);
}

void
Client::sendData(int eventInfo)
{
}
