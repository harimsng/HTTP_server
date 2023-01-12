#ifndef EVENTOBJECT_HPP
#define EVENTOBJECT_HPP

#include "Webserv.hpp"

template <typename IoEventPollerType>
class	EventObjectTemplate
{
public:
	typedef	IoEventPollerType	IoEventPoller;
	typename IoEventPoller::EventStatus
	handleEvent(const typename IoEventPoller::Event& event)
	{
		return handleEventWork(event);
	}

protected:
	~EventObjectTemplate();

private:
	virtual typename IoEventPoller::EventStatus
	handleEventWork(const typename IoEventPoller::Event& event) = 0;
};

struct	IEventObject: public EventObjectTemplate<IO_EVENT_POLLER>
{
protected:
	~IEventObject() {};
};

#endif
