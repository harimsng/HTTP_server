#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <set>

#include "OsDependency.hpp"
#include "Webserv.hpp"
#include "Logger.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "parser/ConfigParser.hpp"
#include "event/Server.hpp"
#include "event/Client.hpp"
#include "event/Cgi.hpp"

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

// member variables
	// std::vector<Server>		m_serverList;

// static members
public:
	static IoEventPoller		s_ioEventPoller;
	static VirtualServerTable	s_virtualServerTable;
	static ListenServerTable	s_listenServerTable;

	static void	registerEvent(int fd, IoEventPoller::e_operation op,
			int filter, EventObject* object);

// friends
//	friend std::ostream&	operator<<(std::ostream& os, const ServerManager& manager);
};

#endif
