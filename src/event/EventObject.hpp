#ifndef EVENTOBJECT_HPP
#define EVENTOBJECT_HPP

#include "OsDependency.hpp"
#include IO_EVENT_HEADER

class	EventObject
{
public:
	enum e_eventStatus
	{
		EVENT_EOF = 0,
		EVENT_NORMAL = 1,
	};

	typedef	IO_EVENT_POLLER	IoEventPoller;

	EventObject();
	EventObject(int fd);
	virtual ~EventObject();

	IoEventPoller::EventStatus	handleReadEvent();
	IoEventPoller::EventStatus	handleWriteEvent();
	IoEventPoller::EventStatus	handleErrorEvent();

protected:
	virtual IoEventPoller::EventStatus	handleReadEventWork() = 0;
	virtual IoEventPoller::EventStatus	handleWriteEventWork() = 0;
	virtual IoEventPoller::EventStatus	handleErrorEventWork() = 0;

public:
	int				m_fd;
	int				m_filter;
	e_eventStatus	m_eventStatus;
};

#endif
