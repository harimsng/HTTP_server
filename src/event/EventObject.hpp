#ifndef EVENTOBJECT_HPP
#define EVENTOBJECT_HPP

#include "OsDependency.hpp"
#include IO_EVENT_HEADER

class	EventObject
{
public:
	enum e_eventStatus
	{
		EVENT_NORMAL = 1,
		EVENT_EOF = 0,
	};

	typedef	IO_EVENT_POLLER	IoEventPoller;

	EventObject();
	EventObject(int fd);
	virtual ~EventObject();

	IoEventPoller::EventStatus	readEventHandler();
	IoEventPoller::EventStatus	writeEventHandler();
	IoEventPoller::EventStatus	errorEventHandler();

protected:
	virtual IoEventPoller::EventStatus	readEventHandlerWork() = 0;
	virtual IoEventPoller::EventStatus	writeEventHandlerWork() = 0;
	virtual IoEventPoller::EventStatus	errorEventHandlerWork() = 0;

public:
	int				m_fd;
	int				m_filter;
	e_eventStatus	m_eventStatus;
};

#endif
