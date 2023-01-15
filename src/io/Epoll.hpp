#ifdef __linux__

# ifndef EPOLL_HPP
# define EPOLL_HPP

# include <sys/epoll.h>

# include "IoMultiplex.hpp"

struct	EpollEvent: public epoll_event
{
	struct	InternalData
	{
		int		fd;
		void*	udata;
	};
	int		getFd() const {return reinterpret_cast<InternalData*>(data.ptr)->fd;}
	int		getInfo() const {return events;}
	void*	getUserDataPtr() const {return reinterpret_cast<InternalData*>(data.ptr)->udata;}

	IoMultiplex<EpollEvent>::e_filters	getFilter() const
	{
		switch (events & (EPOLLIN | EPOLLOUT | EPOLLPRI))
		{
			case EPOLLIN:
				return IoMultiplex<EpollEvent>::READ;
			case EPOLLOUT:
				return IoMultiplex<EpollEvent>::WRITE;
			case EPOLLPRI:
				return IoMultiplex<EpollEvent>::ERROR;
		}
		return IoMultiplex<EpollEvent>::NONE;
	}
	
	int	m_op;
};

class	Epoll: public IoMultiplex<EpollEvent>
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
	virtual void	addWork(int fd, const Event& event);
	virtual void	addWork(int fd, e_operation op, e_filters filter, void* userData = 0);
	virtual const EventList&	pollWork();

private:
// member variables
	int			m_epoll;
	EventList	m_eventList;
};

# endif
#endif
