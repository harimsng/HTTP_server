#ifdef __APPLE__
#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include <sys/event.h>

#include "IIoMultiplex.hpp"

struct	Kevent
{
	int	getFd() const {return m_kevent.ident;}
	int	getFilter() const {return m_kevent.filter;}
	int	getInfo() const {return m_kevent.data;}

	struct kevent	m_kevent;
};

struct	KqueueAttr
{
	typedef Kevent					EventData;
	typedef std::vector<EventData>	EventList;
};

class	Kqueue: public IIoMultiplex<KqueueAttr>
{
// deleted
	Kqueue(Kqueue const& kqueue);
	Kqueue	&operator=(Kqueue const& kqueue);

public:
// constructors & destructor
	Kqueue();
	virtual ~Kqueue();

// member functions
	virtual void	add(int fd, const EventData& event);
	virtual void	add(int fd, e_operation flag, e_filters filter);
	EventData		createEvent(intptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags = 0,
								intptr_t data = 0, void* udata = NULL);
	virtual const EventList&	poll();

private:
// member variables
	int					m_kqueue;
	std::vector<Kevent>	m_changeList;
	std::vector<Kevent>	m_eventList;
	size_t				m_registeredEventSize;
};

#endif
#endif
