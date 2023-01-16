#ifndef KQUEUE_HPP
#define KQUEUE_HPP

#include <sys/event.h>

#include "IoMultiplex.hpp"

class	Kqueue: public IoMultiplex<kevent>
{
// deleted
	Kqueue(Kqueue const& kqueue);
	Kqueue	&operator=(Kqueue const& kqueue);

// types

public:
// constructors & destructor
	Kqueue();
	~Kqueue();

// member functions
	virtual void	addWork(int fd, e_operation op, e_filters filter, void* userData = NULL);
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
