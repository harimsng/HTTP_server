#include <unistd.h>

#include "Kqueue.hpp"

// constructors & destructor
Kqueue::Kqueue()
{
	m_kqueue = kqueue();
	if (m_kqueue < 0)
		throw std::runtime_error("kqueue() fail");
}

Kqueue::~Kqueue()
{
	close(m_kqueue);
}

Kqueue::Kqueue(Kqueue const& kqueue)
{
	(void)kqueue;
	throw std::runtime_error("deleted function Kqueue::Kqueue(Kqueue const&) usage");
}

// operators
Kqueue&
Kqueue::operator=(Kqueue const& kqueue)
{
	(void)kqueue;
	throw std::runtime_error("deleted function Kqueue::operator=(Kqueue const&) usage");
	return *this;
}

void
Kqueue::addWork(int fd, const Event& event)
{
	(void)fd;
	m_changeList.push_back(event);
}

void
Kqueue::addWork(int fd, e_operation op, e_filters filter, void* userData)
{
	static const int16_t	filterTable[3] = {EVFILT_READ, EVFILT_WRITE, EVFILT_EXCEPT};
	static const uint16_t	flagTable[3] = {EV_ADD | EV_ENABLE, EV_DELETE, EV_ADD | EV_ENABLE};
	Kevent	event;

	EV_SET(&event, fd, 0, 0, 0, 0, userData);
	event.flags = flagTable[op - 1];
	for (uint64_t bitmask = 1, count = 0; count < 3; bitmask <<= 1, ++count)
	{
		if ((filter & bitmask) != 0)
		{
			event.filter = filterTable[count];
			m_changeList.push_back(event);
		}
	}
}

Kqueue::Event
Kqueue::createEvent(intptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags,
					intptr_t data, void* udata)
{
	Kevent	newEvent;

	EV_SET(&newEvent, fd, filter, flags, fflags, data, udata);
	return newEvent;
}

const Kqueue::EventList&
Kqueue::pollWork()
{
	const int	maxEvent = 64;
	int			count = 0;
	// static const timespec	timeSpec = {0, 0};

	m_eventList.resize(maxEvent);
	count = kevent(m_kqueue, m_changeList.data(), m_changeList.size(),
				   m_eventList.data(), m_eventList.size(), 0);
	m_changeList.clear();
	if (count < 0)
		throw std::runtime_error("kevent() error");
	m_eventList.resize(count);
	return m_eventList;
}
