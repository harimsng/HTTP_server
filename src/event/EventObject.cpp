#include "Logger.hpp"
#include "EventObject.hpp"

EventObject::~EventObject()
{
//	LOG(DEBUG, "[%d] event destructed", m_fd);
}

EventObject::EventObject()
:	m_filter(0),
	m_eventStatus(EVENT_NORMAL)
{
}

EventObject::EventObject(int fd)
:	m_fd(fd),
	m_filter(0),
	m_eventStatus(EVENT_NORMAL)
{
}

EventObject::IoEventPoller::EventStatus
EventObject::readEventHandler()
{
	return readEventHandlerWork();
}

EventObject::IoEventPoller::EventStatus
EventObject::writeEventHandler()
{
	return writeEventHandlerWork();
}

EventObject::IoEventPoller::EventStatus
EventObject::errorEventHandler()
{
	return errorEventHandlerWork();
}
