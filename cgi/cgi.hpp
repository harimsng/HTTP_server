#ifndef CGI_HPP
#define CGI_HPP

#include <stdexcept>

class	Cgi
{
// deleted
	Cgi	&operator=(Cgi const& cgi);

public:
// constructors & destructor
	Cgi();
	~Cgi();
	Cgi(Cgi const& cgi);

// member functions
	template <typename IoEventPoller>
	typename IoEventPoller::EventStatus	handleEvent(const typename IoEventPoller::EventData& event);
};

template <typename IoEventPoller>
typename IoEventPoller::EventStatus	Cgi::handleEvent(const typename IoEventPoller::EventData& event)
{
	typename IoEventPoller::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventPoller::READ:
			return IoEventPoller::STAT_ERROR;
		case IoEventPoller::WRITE:
			return IoEventPoller::STAT_ERROR;
		default:
			throw std::runtime_error("not handled event filter in Cgi::handleEvent()");
	}
	return IoEventPoller::STAT_ERROR;
}

#endif
