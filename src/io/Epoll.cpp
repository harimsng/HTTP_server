#include <unistd.h>
#include <stdexcept>
#include <errno.h>

#include "Logger.hpp"
#include "event/EventObject.hpp"
#include "Epoll.hpp"

#define MATCH_EVENT(events, event) (((events) & (event)) == (event))

// deleted
Epoll::Epoll(Epoll const& epoll)
{
	(void)epoll;
	throw std::runtime_error("deleted function Epoll::Epoll(Epoll const&) usage");
}

Epoll&
Epoll::operator=(Epoll const& epoll)
{
	(void)epoll;
	throw std::runtime_error("deleted function Epoll::operator=(Epoll const&) usage");
	return *this;
}

// constructors & destructor
Epoll::Epoll()
{
	m_epoll = epoll_create(1);
	if (m_epoll < 0)
		throw std::runtime_error("epoll() fail");
}

Epoll::~Epoll()
{
	close(m_epoll);
}

void
Epoll::addWork(int fd, e_operation op, e_filters filter, EventObject* object)
{
	static const int	operationTable[3] = {EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD};
	static const uint32_t	filterTable[3] = {EPOLLIN, EPOLLOUT, EPOLLERR};
	Event	newEvent;

	::memset(&newEvent, 0, sizeof(newEvent));
	newEvent.data.ptr = object;
	for (int count = 0, bitmask = 0x1; count < 3; ++count, bitmask <<= 1)
	{
		if (filter & bitmask)
			newEvent.events |= filterTable[count];
	}
	if (epoll_ctl(m_epoll, operationTable[op - 1], fd, reinterpret_cast<epoll_event*>(&newEvent)) < 0)
		throw std::runtime_error("epoll_ctl fail");
}

int
Epoll::pollWork()
{
	const int	maxEvent = 64;
	int			count = 0;

	m_eventList.resize(maxEvent);
	count = epoll_wait(m_epoll, m_eventList.data(), m_eventList.size(), 100);
	if (count < 0)
		throw std::runtime_error("epoll() error");
	m_eventList.resize(count);

	if (m_eventList.size() > 0)
	{
		LOG(DEBUG, "%d events polled", m_eventList.size());
	}

	for (size_t i = 0; i < m_eventList.size(); ++i)
	{
		Event&			event = m_eventList[i];
		EventObject*	object = reinterpret_cast<EventObject*>(event.data.ptr);
		int				status = STAT_NORMAL;

		if (MATCH_EVENT(event.events, EPOLLIN))
		{
			object->m_filter = FILT_READ;
			status |= object->handleEvent();
		}
		if (MATCH_EVENT(event.events, EPOLLOUT))
		{
			object->m_filter = FILT_WRITE;
			status |= object->handleEvent();
		}
		if (MATCH_EVENT(event.events, EPOLLERR))
		{
			object->m_filter = FILT_ERROR;
			status |= object->handleEvent();
		}
		if (status == STAT_END)
		{
			LOG(DEBUG, "event(fd:%d) ends", object->m_fd);
			delete object;
		}
	}
	return 0;
}

