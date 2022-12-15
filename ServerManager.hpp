#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <sys/event.h>

#include <vector>
#include <map>

#include "Server.hpp"

class	ServerManager
{
	struct	EventTarget;

public:
// constructors & destructor
	ServerManager();
	~ServerManager();
	ServerManager(ServerManager const& serverManager);

// operators
	ServerManager	&operator=(ServerManager const& serverManager);

// member functions
	void	parse(const char* path);
	void	initServers();
	void	run();
	void	processEvents(struct kevent& event);
	void	handleClientEvent(const struct kevent& event);
	void	handleServerEvent(const struct kevent& event);

private:
// member variables
	std::vector<struct kevent>	m_changeList;
	std::vector<struct kevent>	m_eventList;
	std::vector<Server>			m_serverList;
	std::map<int, EventTarget>	m_eventTargetMap;
	int							m_kqueue;

	friend std::ostream&	operator<<(std::ostream& os, const ServerManager& manager);
};

#endif
