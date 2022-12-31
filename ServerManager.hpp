#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>
#include <map>
#include <iostream>

#include "Logger.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "cgi/Cgi.hpp"
#include "parser/ConfigParser.hpp"
//#include "socket/ServerSocket.hpp"

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
	template <typename ObjectType>
	void	processEventObject(const EventData& event, EventObject& eventObject);

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

	static std::map<int, EventObject>		s_eventObjectMap;
	static IoEventPoller					s_ioEventPoller;

// friends
	friend std::ostream&	operator<<(std::ostream& os, const ServerManager& manager);
};

/*
 * Definitions
 */

template <typename IoEventPoller>
std::map<int, EventObject>	ServerManager<IoEventPoller>::s_eventObjectMap;

template <typename IoEventPoller>
IoEventPoller	ServerManager<IoEventPoller>::s_ioEventPoller;

template <typename IoEventPoller>
ServerManager<IoEventPoller>::ServerManager()
{
}

template <typename IoEventPoller>
ServerManager<IoEventPoller>::~ServerManager()
{
	for (std::map<int, EventObject>::iterator itr = s_eventObjectMap.begin();
		 itr != s_eventObjectMap.end();
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
		m_serverList[i].template initServer<IoEventPoller>();

	Logger::log(Logger::INFO, "%zu servers are initiated", m_serverList.size());
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::run() try
{
	initServers();

	while (1)
	{
		const EventList&	eventList = s_ioEventPoller.poll();
		processEvents(eventList);
	}
	Logger::log(Logger::INFO, "%zu servers exited", m_serverList.size());
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
	for (size_t i = 0; i < events.size(); ++i)
	{
		const EventData&	event = events[i];
		EventObject&		eventObject = s_eventObjectMap[event.getFd()];

		switch (eventObject.type)
		{
			case EventObject::SERVER:
				processEventObject<Server>(event, eventObject);
				break;
			case EventObject::CLIENT:
				processEventObject<Client>(event, eventObject);
				break;
			case EventObject::CGI:
				processEventObject<Cgi>(event, eventObject);
				break;
			default:
				throw std::logic_error("unhandled event type in ServerManager::processEvents");
		}
	}
}

template <typename IoEventPoller>
template <typename ObjectType>
void
ServerManager<IoEventPoller>::processEventObject(const EventData& event, EventObject& eventObject)
{
	ObjectType*	object = reinterpret_cast<ObjectType*>(eventObject.object);

	if (object->template handleEvent<IoEventPoller>(event) == IoEventPoller::END)
	{
		object->~ObjectType();
		eventObject.type = EventObject::EMPTY;
	}
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
	EventObject	et;

	et.type = type;
	switch (type)
	{
		case EventObject::SERVER:
			et.object = reinterpret_cast<void*>(server);
			break;
		case EventObject::CLIENT:
			if (s_eventObjectMap.count(fd) == 0)
				et.object = operator new(ALLOC_SIZE);

			else if (type != s_eventObjectMap[fd].type)
				throw std::runtime_error("addEventObject(): previous object has not been cleaned");

			new (et.object) Client(*server, fd);
			break;
		case EventObject::CGI:
			if (s_eventObjectMap.count(fd) == 0)
				et.object = operator new(ALLOC_SIZE);

			else if (type != s_eventObjectMap[fd].type)
				throw std::runtime_error("addEventObject(): previous object has not been cleaned");

			new (et.object) Cgi();
			break;

		default:
			;
	}
	s_eventObjectMap[fd] = et;
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::removeEventObject(int fd)
{
	EventObject&	target = s_eventObjectMap[fd];

	switch (target.type)
	{
		case EventObject::CLIENT:
			Client* client;

			client = reinterpret_cast<Client*>(target.object);
			client->~Client();
			break;
		case EventObject::CGI:
			Cgi*	cgi;

			cgi = reinterpret_cast<Cgi*>(target.object);
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
