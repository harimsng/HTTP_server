#ifdef __APPLE__

# ifndef KQUEUE_HPP
# define KQUEUE_HPP

# include <sys/event.h>

# include <vector>

# include "IIoMultiplex.hpp"

struct	Kevent: public kevent
{
	int		getFd() const {return ident;}
	int		getInfo() const {return data;}
	void*	getUserDataPtr() const {return udata;}
	EventProperties::e_filters	getFilter() const
	{
		switch (filter)
		{
			case EVFILT_READ:
				return EventProperties::READ;
			case EVFILT_WRITE:
				return EventProperties::WRITE;
			default:
				return EventProperties::FILT_ERROR;
		}
	}
};

struct	KqueueAttr
{
	typedef Kevent				Event;
	typedef std::vector<Event>	EventList;
};

class	Kqueue: public IIoMultiplex<KqueueAttr>
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
	virtual void	add(int fd, const Event& event);
	virtual void	add(int fd, e_operation op, e_filters filter, void* userData = NULL);
	Event			createEvent(intptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags = 0,
							intptr_t data = 0, void* udata = NULL);
	virtual const EventList&	poll();

private:
// member variables
	int					m_kqueue;
	std::vector<Kevent>	m_changeList;
	std::vector<Kevent>	m_eventList;
	size_t				m_registeredEventSize;
};

# endif

#endif
