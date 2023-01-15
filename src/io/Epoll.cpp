#include <unistd.h>
#include <stdexcept>

#include "Epoll.hpp"

// deleted
Epoll::Epoll(Epoll const& epoll)
{
	(void)epoll;
	throw std::runtime_error("deleted function Epoll::Epoll(Epoll const&) usage");
}

// operators
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
Epoll::addWork(int fd, const Event& event)
{
	Event*	newEvent = const_cast<Event*>(&event);

	epoll_ctl(m_epoll, event.m_op, fd, reinterpret_cast<epoll_event*>(newEvent));
}

void
Epoll::addWork(int fd, e_operation op, e_filters filter, void* userData)
{
	static const int	operationTable[3] = {EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD};
	static const uint32_t	filterTable[3] = {EPOLLIN, EPOLLOUT, EPOLLERR};
	Event	newEvent;
	Event::InternalData*	internal;

	// TODO: decide when to release
	if (op == ADD)
		newEvent.data.ptr = new Event::InternalData;
	newEvent.m_op = operationTable[op - 1];
	internal = reinterpret_cast<Event::InternalData*>(newEvent.data.ptr);
	internal->fd = fd;
	internal->udata = userData;
	for (int count = 0, bitmask = 0x1; count < 3; ++count, bitmask <<= 1)
	{
		if (filter & bitmask)
			newEvent.events |= filterTable[count];
	}
	epoll_ctl(m_epoll, newEvent.m_op, fd, reinterpret_cast<epoll_event*>(&newEvent));
}

const Epoll::EventList&
Epoll::pollWork()
{
	const int	maxEvent = 64;
	int			count = 0;
	// static const timespec	timeSpec = {0, 0};

	m_eventList.resize(maxEvent);
	count = epoll_wait(m_epoll, m_eventList.data(), m_eventList.size(), 0);
	if (count < 0)
		throw std::runtime_error("epoll() error");
	m_eventList.resize(count);
	return m_eventList;
}
