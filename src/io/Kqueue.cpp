#include <unistd.h>

#include "Kqueue.hpp"

// deleted
Kqueue::Kqueue(Kqueue const& kqueue)
{
	(void)kqueue;
	throw std::runtime_error("deleted function Kqueue::Kqueue(Kqueue const&) usage");
}

Kqueue&
Kqueue::operator=(Kqueue const& kqueue)
{
	(void)kqueue;
	throw std::runtime_error("deleted function Kqueue::operator=(Kqueue const&) usage");
	return *this;
}

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

void
Kqueue::addWork(int fd, e_operation op, e_filters filter, void* userData)
{
	static const int16_t	filterTable[3] = {EVFILT_READ, EVFILT_WRITE, EVFILT_EXCEPT};
	static const uint16_t	operationTable[3] = {EV_ADD | EV_ENABLE, EV_DELETE, EV_ADD | EV_ENABLE};
	Kevent	event;

	EV_SET(&event, fd, 0, 0, 0, 0, userData);
	event.flags = operationTable[op - 1];
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

int
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

	if (m_eventList.size() > 0)
	{
		LOG(DEBUG, "%d events polled", m_eventList.size());
	}

	for (size_t i = 0; i < m_eventList.size(); ++i)
	{
		Event&			event = m_eventList[i];
		EventObject*	object = reinterpret_cast<EventObject*>(event.data.ptr);

		// TODO: divide by filter
		EventStatus status = object->handleEvent(event);
		if (status == END)
		{
			LOG(DEBUG, "event(fd:%d) ends", object->m_fd);
			// INFO: is this right?
			close(object->m_fd);
			delete object;
//			registerEvent(event.getFd(), IoEventPoller::DELETE, IoEventPoller::NONE, NULL);
		}
	}
	return 0;
}
