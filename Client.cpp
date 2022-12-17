#include <sys/event.h>

#include "Client.hpp"

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
{
	(void)client;
	throw logic_error("Client(Client const&) must not be used");
}

// operators
Client&	Client::operator=(Client const& client)
{
	(void)client;
	throw logic_error("operator=(Client const&) must not be used");
	return *this;
}
