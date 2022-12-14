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
Kqueue::set(int fd, const Kevent& event)
{
	(void)fd;
	m_eventList.push_back(event);
}

void
Kqueue::set(int fd, e_flags flag, e_filters filter)
{
	static int16_t	filterTable[3] = {EVFILT_READ, EVFILT_WRITE, EVFILT_EXCEPT};
	static uint16_t	flagTable[3] = {EV_ADD | EV_ENABLE, EV_DELETE, EV_ADD | EV_ENABLE};
	Kevent			newEvent;

	EV_SET(&newEvent, fd, 0, 0, 0, 0, NULL);
	newEvent.flags = flagTable[flag - 1];
	for (uint64_t bitmask = 1, count = 0; count < 3; bitmask <<= 1, ++count)
	{
		if ((filter & bitmask) != 0)
		{
			newEvent.filter = filterTable[count];
			m_eventList.push_back(newEvent);
		}
	}
}

Kqueue::EventData
Kqueue::createEvent(intptr_t fd, int16_t filter, uint16_t flags, uint32_t fflags,
					intptr_t data, void* udata)
{
	struct Kevent	newEvent;

	EV_SET(&newEvent, fd, filter, flags, fflags, data, udata);
	return newEvent;
}

const Kqueue::EventList&
Kqueue::poll()
{
	int						count = 0;
	static const timespec	timeSpec = {0, 0};

	count = kevent(m_kqueue, m_eventList.data(), m_eventList.size(), m_eventList.data(), m_eventList.size(), &timeSpec);
	if (count < 0)
		throw std::runtime_error("kevent() error");
	return m_eventList;
}
