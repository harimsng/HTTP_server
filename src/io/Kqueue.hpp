#ifdef __APPLE__

# ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <sys/event.h>

# include "IoMultiplex.hpp"

struct	Kevent: public kevent
{
	int		getFd() const {return ident;}
	int		getInfo() const {return data;}
	void*	getUserDataPtr() const {return udata;}
	IoMultiplex<Kevent>::e_filters	getFilter() const
	{
		switch (filter)
		{
			case EVFILT_READ:
				return IoMultiplex<Kevent>::READ;
			case EVFILT_WRITE:
				return IoMultiplex<Kevent>::WRITE;
			default:
				return IoMultiplex<Kevent>::FILT_ERROR;
		}
	}
};

class	Kqueue: public IoMultiplex<Kevent>
{
// deleted
	Kqueue(Kqueue const& kqueue);
	Kqueue	&operator=(Kqueue const& kqueue);

// types

public:
// constructors & destructor
	Kqueue();
	virtual ~Kqueue();

// member functions
	virtual void	addWork(int fd, const Event& event);
	virtual void	addWork(int fd, e_operation op, e_filters filter, void* userData = NULL);
	Event			createEvent(intptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags = 0,
							intptr_t data = 0, void* udata = NULL);
	virtual const EventList&	pollWork();

private:
// member variables
	int		m_kqueue;
};

# endif

#endif
