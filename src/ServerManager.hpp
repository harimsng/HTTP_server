#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Webserv.hpp"
#include IO_EVENT_HEADER

class	ServerManager
{
// types
	typedef IO_EVENT_POLLER				IoEventPoller;
	typedef IoEventPoller::Event		Event;
	typedef IoEventPoller::EventList	EventList;

// deleted
	ServerManager(ServerManager const& serverManager) {(void)serverManager;}
	ServerManager	&operator=(ServerManager const& serverManager) {(void)serverManager; return *this;}
public:
// constructors & destructor
	ServerManager();
	~ServerManager();

// member functions
	void	parseConfig(const char* path);
	void	run();

private:
	void	initServers();

// static members
public:
	static IoEventPoller		s_ioEventPoller;
	static VirtualServerTable	s_virtualServerTable;
	static ListenServerTable	s_listenServerTable;

	static void	registerEvent(int fd, IoEventPoller::e_operation op,
			IoEventPoller::e_filters filter, EventObject* object);

};

#endif
