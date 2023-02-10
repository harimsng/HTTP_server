#include <unistd.h>

#include "Logger.hpp"
#include "event/EventObject.hpp"
#include "io/Kqueue.hpp"

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
Kqueue::addWork(int fd, e_operation op, int filter, EventObject* userData)
{
	static const int16_t	filterTable[3] = {EVFILT_READ, EVFILT_WRITE, EVFILT_EXCEPT};
	static const uint16_t	operationTable[3] = {EV_ADD, EV_DELETE, EV_ADD};
	Kevent		event;

	EV_SET(&event, fd, 0, 0, 0, 0, userData);
	event.flags = operationTable[op - 1];
	
	// it assumes that events which have a same fd share a same EventObject instance.
	// TODO: m_filter adjusting for epoll
	switch (op)
	{
		case OP_ADD:
			userData->m_filter |= filter;
			break;
		case OP_DELETE:
			userData->m_filter &= ~filter;
			break;
		case OP_MODIFY:
			userData->m_filter = filter;
			break;
	}

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
	struct timespec	interval;

	interval.tv_sec = 0;
	interval.tv_nsec = 10 * 1000 * 1000;
	m_eventList.resize(maxEvent);
	count = kevent(m_kqueue, m_changeList.data(), m_changeList.size(),
				   m_eventList.data(), m_eventList.size(), &interval);
	m_changeList.clear();
	if (count < 0)
		throw std::runtime_error("kevent() error");
	m_eventList.resize(count);

	for (size_t i = 0; i < m_eventList.size(); ++i)
	{
		Event&			event = m_eventList[i];
		EventObject*	object = reinterpret_cast<EventObject*>(event.udata);

		// NOTE: add eof test for epoll
		if (TEST_BITMASK(event.flags, EV_EOF))
		{
			object->m_eventStatus = EventObject::EVENT_EOF;
		}

		EventStatus status;

		switch (event.filter)
		{
			case EVFILT_READ:
				status = object->handleReadEvent();
				break;
			case EVFILT_WRITE:
				status = object->handleWriteEvent();
				break;
			case EVFILT_EXCEPT:
				status = object->handleErrorEvent();
				break;
			default:
				throw std::runtime_error("not handled event filter in Kqueue::pollWork()");
		}

		if (status == STAT_END)
		{
			delete object;
			break;
		}
	}
	m_eventList.clear();
	return 0;
}
