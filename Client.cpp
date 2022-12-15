#include <sys/event.h>
#include <unistd.h>
#include <stdexcept>

#include "Client.hpp"

using namespace	std;

// constructors & destructor
Client::Client(Server& server)
:	m_server(&server)
{
}

Client::~Client()
{
	close(m_socket);
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

void
Client::handleEvent(const struct kevent& event)
{
	int16_t		filter = event.filter;

	switch (filter)
	{
		case EVFILT_READ:
		case EVFILT_WRITE:
		case EVFILT_EXCEPT:
		default:
			throw runtime_error("not handled event filter in Client::handleEvent()");
	}
}
