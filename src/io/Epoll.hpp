#ifdef __linux__

# ifndef EPOLL_HPP
# define EPOLL_HPP

# include <sys/epoll.h>

# include "IoMultiplex.hpp"

class	EventObject;

class	Epoll: public IoMultiplex<epoll_event>
{
// deleted
	Epoll(Epoll const& epoll);
	Epoll	&operator=(Epoll const& epoll);

// types

public:
// constructors & destructor
	Epoll();
	~Epoll();

// member functions
	virtual void	addWork(int fd, e_operation op, e_filters filter, EventObject* object);
	virtual int		pollWork();

private:
// member variables
	int			m_epoll;
	EventList	m_eventList;
};

# endif
#endif
