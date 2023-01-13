#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <set>

#include "Webserv.hpp"
#include "Logger.hpp"
#include "exception/HttpErrorHandler.hpp"
#include "parser/ConfigParser.hpp"
#include "event/Server.hpp"
#include "event/Client.hpp"
#include "event/Cgi.hpp"

static const std::size_t	g_objectSize = (std::max(sizeof(Client), sizeof(Cgi)));

// IoEventPoller is class to encapsulate IO multiplexing system calls and associated types.
template <typename IoEventPoller>
class	ServerManager
{
// types
	typedef typename IoEventPoller::Event		Event;
	typedef typename IoEventPoller::EventList	EventList;
	typedef IEventObject						EventObject;

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
	void	processEvents(const EventList& events);
	template <typename ObjectType>
	void	processEventObject(const Event& event, EventObject& eventObject);

// member variables
	std::vector<Server>		m_serverList;

// static members
private:
	static IoEventPoller				s_ioEventPoller;
	static VirtualServerTable			s_virtualServerTable;
	static ListenServerTable			s_listenServerTable;

public:
	static void registerEvent(int fd, typename IoEventPoller::e_operation op,
			typename IoEventPoller::e_filters filter, EventObject* eventObject);

// friends
//	friend std::ostream&	operator<<(std::ostream& os, const ServerManager& manager);
};


// static member definitions
template <typename IoEventPoller>
IoEventPoller	ServerManager<IoEventPoller>::s_ioEventPoller;

template <typename IoEventPoller>
VirtualServerTable	ServerManager<IoEventPoller>::s_virtualServerTable;

template <typename IoEventPoller>
ListenServerTable	ServerManager<IoEventPoller>::s_listenServerTable;


// member function definitions
template <typename IoEventPoller>
ServerManager<IoEventPoller>::ServerManager()
{
}

template <typename IoEventPoller>
ServerManager<IoEventPoller>::~ServerManager()
{
}


template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::parseConfig(const char* path)
{
	ConfigParser	configParser;

	configParser.init(path, s_virtualServerTable);
	configParser.parse();
	Logger::log(Logger::INFO, "parsing configuration file finished");
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::initServers()
{
	set<uint64_t>	portSet;

	for (VirtualServerTable::iterator itr = s_virtualServerTable.begin();
		 itr != s_virtualServerTable.end();
		 ++itr)
	{
		portSet.insert(itr->first & 0xffff);
	}

	for (set<uint64_t>::iterator itr = portSet.begin();
		 itr != portSet.end();
		 ++itr)
	{
		Server*	newServer = new Server(*itr);

		newServer->initServer();
		ServerManager<IoEventPoller>::registerEvent(newServer->m_fd, IoEventPoller::ADD,
			IoEventPoller::READ, newServer);
		s_listenServerTable[*itr] = newServer;
	}

	Logger::log(Logger::INFO, "%zu listen servers are initiated", s_listenServerTable.size());
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
	Logger::log(Logger::INFO, "%zu listen servers exited", s_listenServerTable.size());
}
// TODO: cleanup
catch (std::runtime_error& e)
{
	Logger::log(Logger::ERROR, "%s", e.what());
}
catch (HttpErrorHandler& e)
{
	Logger::log(Logger::ERROR, "%s", e.getErrorMessage().data());
}
catch (...)
{
	Logger::log(Logger::ERROR, "unexpected error");
}


template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::processEvents(const EventList& events)
{
	for (size_t i = 0; i < events.size(); ++i)
	{
		const Event&	event = events[i];
		void*			udata = const_cast<void*>(event.getUserDataPtr());

		typename IoEventPoller::EventStatus status =
			reinterpret_cast<EventObject*>(udata)->handleEvent(event);
		if (status == IoEventPoller::END)
		{
			LOG(DEBUG, "event(fd:%d) ends", event.getFd());
			// INFO: is this right?
			close(event.getFd());
//			registerEvent(event.getFd(), IoEventPoller::DELETE, IoEventPoller::NONE, NULL);
		}
	}
}

template <typename IoEventPoller>
void
ServerManager<IoEventPoller>::registerEvent(int fd, typename IoEventPoller::e_operation op,
		typename IoEventPoller::e_filters filter, EventObject* eventObject)
{
	s_ioEventPoller.add(fd, op, filter, eventObject);
}

#endif
