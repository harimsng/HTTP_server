#include <sys/event.h>

#include "Client.hpp"

using namespace	std;

// constructors & destructor
Client::Client(Server& server, int fd)
:	m_server(&server),
	m_socket(fd),
	m_stream()
{
}

Client::~Client()
{
}

Client::Client(Client const& client)
:	m_server(client.m_server),
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
	3
}

void
Client::sendData(int eventInfo)
{
}

