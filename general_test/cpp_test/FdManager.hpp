#ifndef FDMANAGER_HPP
#define FDMANAGER_HPP

#include <sys/socket.h>

class	FdManager
{
	FdManager(FdManager const& fdManager);
	FdManager	&operator=(FdManager const& fdManager);

public:
// constructors & destructor
	FdManager();
	~FdManager();

// operators

// member functions
	void	addWriteFd(int fd);
	void	addReadFd(int fd);
	void	addExceptionFd(int fd);

	void	init();
private:
// member variables
	fd_set		m_readSet;
	fd_set		m_writeSet;
	fd_set		m_exceptionSet;

	int			m_maxFd;
};

#endif
