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
EventObject::handleReadEvent()
{
	return handleReadEventWork();
}

EventObject::IoEventPoller::EventStatus
EventObject::handleWriteEvent()
{
	return handleWriteEventWork();
}

EventObject::IoEventPoller::EventStatus
EventObject::handleErrorEvent()
{
	return handleErrorEventWork();
}
