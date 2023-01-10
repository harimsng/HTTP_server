#ifndef EVENTOBJECT_HPP
#define EVENTOBJECT_HPP

#include "Webserv.hpp"

template <typename IoEventPollerType>
struct	EventObjectTemplate
{
	typedef	IoEventPollerType	IoEventPoller;
	virtual typename IoEventPoller::EventStatus
	handleEvent(const typename IoEventPoller::Event& event) = 0;
};

struct	IEventObject: public EventObjectTemplate<IO_EVENT_POLLER>
{
};

#endif
