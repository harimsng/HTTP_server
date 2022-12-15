#ifndef CLIENT_HPP
#define CLIENT_HPP

class	Server;

class	Client
{
// deleted
	Client(Client const& client);
	Client	&operator=(Client const& client);

public:
// constructors & destructor
	Client(Server& server);
	~Client();

	void	handleEvent(const struct kevent& event);

// member functions

// member variables
	int				m_socket;
	const Server*	m_server;

};

#endif
