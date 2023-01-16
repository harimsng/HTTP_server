#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include <sys/event.h>

#include "IoMultiplex.hpp"

class	EventObject;

class	Kqueue: public IoMultiplex<struct kevent>
{
	typedef struct kevent	Kevent;
// deleted
	Kqueue(Kqueue const& kqueue);
	Kqueue	&operator=(Kqueue const& kqueue);

public:
// constructors & destructor
	Kqueue();
	~Kqueue();

// member functions
	virtual void	addWork(int fd, e_operation op, e_filters filter, EventObject* userData = NULL);
	Event			createEvent(intptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags = 0,
							intptr_t data = 0, void* udata = NULL);
	virtual int		pollWork();

private:
// member variables
	int			m_kqueue;
	EventList	m_eventList;
	EventList	m_changeList;
};

#endif
