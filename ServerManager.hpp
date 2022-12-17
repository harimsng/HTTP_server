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

// IoEventHandler is class to encapsulate IO multiplexing system calls and associated types.
template <typename IoEventHandler>
class	ServerManager
{
// types
	typedef typename IoEventHandler::EventData	EventData;
	typedef typename IoEventHandler::EventList	EventList;

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

// member variables
	std::vector<Server>		m_serverList;

public:
// static members
	static void	addEventTarget(typename EventTarget::e_type type, int fd, void* target);
	static void	removeEventTarget(int fd);

	static std::map<int, EventTarget>		s_eventTargetMap;
	static IoEventHandler					s_ioEventHandler;

// friends
	friend std::ostream&	operator<<(std::ostream& os, const ServerManager& manager);
};

/*
 * Definitions
 */

template <typename IoEventHandler>
std::map<int, EventTarget>	ServerManager<IoEventHandler>::s_eventTargetMap;

template <typename IoEventHandler>
IoEventHandler	ServerManager<IoEventHandler>::s_ioEventHandler;

template <typename IoEventHandler>
ServerManager<IoEventHandler>::ServerManager()
{
}

template <typename IoEventHandler>
ServerManager<IoEventHandler>::~ServerManager()
{
	for (std::map<int, EventTarget>::iterator itr = s_eventTargetMap.begin();
		 itr != s_eventTargetMap.end();
		 ++itr)
		removeEventTarget(itr->first);
}

template <typename IoEventHandler>
void
ServerManager<IoEventHandler>::parse(const char* path) try
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

template <typename IoEventHandler>
void
ServerManager<IoEventHandler>::initServers()
{
	for (size_t i = 0; i < m_serverList.size(); i++)
		m_serverList[i].initServer<IoEventHandler>();

	Logger::log(Logger::INFO, "%zu servers are initiated", m_serverList.size());
}

template <typename IoEventHandler>
void
ServerManager<IoEventHandler>::run() try
{
	initServers();

	while (1)
	{
		EventList&	eventList = s_ioEventHandler.poll();
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

template <typename IoEventHandler>
void
ServerManager<IoEventHandler>::processEvents(const EventList& events)
{
	for (int i = 0; i < events.size(); ++i)
	{
		const EventData&	event = events[i];
		EventTarget&		target = s_eventTargetMap[event.getFd()];

		switch (target.type)
		{
			case EventTarget::SERVER:
				// declaration with initialization is forbidden inside switch case
				Server* server;

				server = reinterpret_cast<Server*>(target.target);
				server->handleEvent(event);
				break;
			case EventTarget::CLIENT:
				Client*	client;

				client = reinterpret_cast<Client*>(target.target);
				client->handleEvent(event);
				break;
			case EventTarget::CGI:
				Cgi*	cgi;

				cgi = reinterpret_cast<Cgi*>(target.target);
				cgi->handleEvent(event);
				break;
			default:
				throw std::logic_error("unhandled event type in ServerManager::processEvents");
		}
	}
}

template <typename IoEventHandler>
void
ServerManager<IoEventHandler>::addEventTarget(typename EventTarget::e_type type, int fd, void* target)
{
	EventTarget	et;

	et.type = type;
	et.target = target;
	s_eventTargetMap[fd] = et;
}

template <typename IoEventHandler>
void
ServerManager<IoEventHandler>::removeEventTarget(int fd)
{
	EventTarget&	target = s_eventTargetMap[fd];

	switch (target.type)
	{
		case EventTarget::CLIENT:
			delete reinterpret_cast<Client*>(target.target);
			break;
		case EventTarget::CGI:
			delete reinterpret_cast<Cgi*>(target.target);
			break;
		default:
			;
	}
	s_eventTargetMap.erase(fd);
}

template <typename IoEventHandler>
std::ostream&	operator<<(std::ostream& os, const ServerManager<IoEventHandler>& manager)
{
	for (uint32_t i = 0; i < manager.m_serverList.size(); ++i)
		os << manager.m_serverList[i] << '\n';
	return os;
}

#endif
