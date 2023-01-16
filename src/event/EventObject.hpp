#ifndef EVENTOBJECT_HPP
#define EVENTOBJECT_HPP

#include "OsDependency.hpp"

class	EventObject
{
public:
	typedef	IO_EVENT_POLLER	IoEventPoller;

	EventObject() {};
	virtual ~EventObject() {};

	IoEventPoller::EventStatus	handleEvent() {return handleEventWork();}

private:
	virtual IoEventPoller::EventStatus	handleEventWork() = 0;

public:
	int							m_fd;
	IoEventPoller::e_filters	m_filter;
};

#endif
