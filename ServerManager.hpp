#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>
#include <map>
#include <iostream>

#include "Server.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "cgi/Cgi.hpp"
#include "io/Kqueue.hpp"
#include "parser/ConfigParser.hpp"
#include "parser/ServerParser.hpp"
#include "parser/LocationParser.hpp"
#include "socket/ServerSocket.hpp"

#define ALLOC_SIZE (std::max(sizeof(Client), sizeof(Cgi)))

// IoEventPoller is class to encapsulate IO multiplexing system calls and associated types.
template <typename IoEventPoller>
class	ServerManager
{
// types
	typedef typename IoEventPoller::EventData	EventData;
	typedef typename IoEventPoller::EventList	EventList;

// deleted
	ServerManager(ServerManager const& serverManager) {(void)serverManager;}
	ServerManager	&operator=(ServerManager const& serverManager) {(void)serverManager; return *this;}
public:
// constructors & destructor
	ServerManager();
	~ServerManager();

// operators

// member functions
	void	parse(const char* path);
	void	run();

private:
	void	initServers();
	void	processEvents(const EventList& events);
	template <typename TargetType>
	void	processEventObject(const EventData& event, TargetType* target);

// member variables
	std::vector<Server>		m_serverList;

public:
// static members
	static void registerEvent(int fd,
			typename IoEventPoller::e_operation op,
			typename IoEventPoller::e_filters filter);
	static void	addEventObject(typename EventObject::e_type type,
			int fd, Server* target);
	static void	removeEventObject(int fd);

	static std::map<int, EventObject>		s_eventTargetMap;
	static IoEventPoller					s_ioEventPoller;

// friends
	friend std::ostream&	operator<<(std::ostream& os, const ServerManager& manager);
};

/*
 * Definitions
 */

template <typename IoEventPoller>
std::map<int, EventObject>	ServerManager<IoEventPoller>::s_eventTargetMap;

template <typename IoEventPoller>
IoEventPoller	ServerManager<IoEventPoller>::s_ioEventPoller;

template <typename IoEventPoller>
ServerManager<IoEventPoller>::ServerManager()
{
}

template <typename IoEventPoller>
ServerManager<IoEventPoller>::~ServerManager()
{
	for (std::map<int, EventObject>::iterator itr = s_eventTargetMap.begin();
		 itr != s_eventTargetMap.end();
		 ++itr)
		removeEventObject(itr->first);
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::parse(const char* path) try
{
	ConfigParser	configParser;

	configParser.init(path);
	configParser.parse(m_serverList);
	Logger::log(Logger::INFO, "configuration file parsing finished");
}
catch (std::exception& e)
{
	std::cout << e.what() << '\n';
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::initServers()
{
	for (size_t i = 0; i < m_serverList.size(); i++)
		m_serverList[i].initServer<IoEventPoller>();

	Logger::log(Logger::INFO, "%zu servers are initiated", m_serverList.size());
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::run() try
{
	initServers();

	while (1)
	{
		EventList&	eventList = s_ioEventPoller.poll();
		processEvents(eventList);
	}
	Logger::log(Logger::INFO, "%zu servers are exiting", m_serverList.size());
}
catch (std::runtime_error& e)
{
	Logger::log(Logger::ERROR, "%s", e.what());
}
catch (...)
{
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::processEvents(const EventList& events)
{
	for (int i = 0; i < events.size(); ++i)
	{
		const EventData&	event = events[i];
		EventObject&		target = s_eventTargetMap[event.getFd()];
		int					result = 0;

		switch (target.type)
		{
			case EventObject::SERVER:
				result = processTargetEvent(event, reinterpret_cast<Server*>(target.target));
				break;
			case EventObject::CLIENT:
				result = processTargetEvent(event, reinterpret_cast<Client*>(target.target));
				break;
			case EventObject::CGI:
				result = processTargetEvent(event, reinterpret_cast<Cgi*>(target.target));
				break;
			default:
				throw std::logic_error("unhandled event type in ServerManager::processEvents");
		}
		if (result == -1)
			target.type = EventObject::EMPTY;
	}
}

template <typename IoEventPoller>
template <typename TargetType>
void
ServerManager<IoEventPoller>::processEventObject(const EventData& event, TargetType* target)
{
	if (target->handleEvent(event) == EventStatus::EOF)
	{
		target->~TargetType();
		return -1;
	}
	return 0;
}


template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::registerEvent(int fd, typename IoEventPoller::e_operation op,
		typename IoEventPoller::e_filters filter)
{
	s_ioEventPoller.add(fd, op, filter);
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::addEventObject(typename EventObject::e_type type, int fd, Server* server)
{
	EventObject			et;

	et.type = type;
	switch (type)
	{
		case EventObject::SERVER:
			et.target = reinterpret_cast<void*>(server);
			break;
		case EventObject::CLIENT:
			if (s_eventTargetMap.count(fd) == 0)
				et.target = operator new(ALLOC_SIZE);
			else if (type != s_eventTargetMap[fd].type)
				throw std::runtime_error("addEventObject(): previous object has not been cleaned");
			new (et.target) Client(*server, fd);
			break;
		case EventObject::CGI:
			if (s_eventTargetMap.count(fd) == 0)
				et.target = operator new(ALLOC_SIZE);
			else if (type != s_eventTargetMap[fd].type)
				throw std::runtime_error("addEventObject(): previous object has not been cleaned");
			new (et.target) Cgi(*server, fd);
			break;
		default:
			;
	}
	s_eventTargetMap[fd] = et;
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::removeEventObject(int fd)
{
	EventObject&	target = s_eventTargetMap[fd];

	switch (target.type)
	{
		case EventObject::CLIENT:
			Client* client;

			client = reinterpret_cast<Client*>(target.target);
			client->~Client();
			break;
		case EventObject::CGI:
			Cgi*	cgi;

			cgi = reinterpret_cast<Cgi*>(target.target);
			cgi->~Cgi();
			break;
		default:
			;
	}
	target.type = EventObject::EMPTY;
}

template <typename IoEventPoller>
std::ostream&	operator<<(std::ostream& os, const ServerManager<IoEventPoller>& manager)
{
	for (uint32_t i = 0; i < manager.m_serverList.size(); ++i)
		os << manager.m_serverList[i] << '\n';
	return os;
}

#endif
