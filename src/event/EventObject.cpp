#include "Logger.hpp"
#include "EventObject.hpp"

EventObject::~EventObject()
{
	LOG(DEBUG, "[%d] event destructed", m_fd);
}

EventObject::IoEventPoller::EventStatus
EventObject::handleEvent()
{
	return handleEventWork();
}
