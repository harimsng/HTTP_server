#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/select.h>

#include <vector>
#include <map>

#include "Connection.hpp"

class	Server
{
	Server	&operator=(Server const& server);

// constructors & destructor
	void	init();
public:
	Server(int listenSocket);
	Server(Server const& server);
	~Server();

// operators

// member functions
	int		acceptConnection(fd_set* readSet);
	void	readOperation(fd_set* fdSet);
	void	writeOperation(fd_set* fdSet);
private:
	int		getConnectionIdx(fd_set* fdSet);

// member variables
	int 									m_listenSocket;
	std::vector<std::pair<int, Connection>>	m_connectionVec;
};

#endif
