#include <iostream>

#include "Logger.hpp"
#include "parser/config_parser/ConfigParser.hpp"
#include "parser/config_parser/ServerParser.hpp"
#include "parser/config_parser/LocationParser.hpp"
#include "socket/ServerSocket.hpp"
#include "ServerManager.hpp"

using namespace std;

// constructors & destructor
ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
}

void
ServerManager::parse(const char* path) try
{
	ConfigParser	configParser;

	configParser.init(path);
	configParser.parse(m_serverList);
	Logger::log(Logger::INFO, "configuration file parsing finished");
}
catch (std::exception& e)
{
	cout << e.what() << '\n';
}

void
ServerManager::initServers()
{
	for (size_t i = 0; i < m_serverList.size(); i++)
	{
		Server& server = m_serverList[i];
		int		fd = server.initServer();

		addEventTarget(fd, EventTarget::SERVER, &server);
	}
}

void
ServerManager::run() try
{
	static const timespec	timeout = {0, 100};
	int						eventCount;

	m_kqueue = kqueue();
	Logger::log(Logger::INFO, "%zu servers are initiated", m_serverList.size());
	while (1)
	{
		m_eventList.resize(m_eventTargetMap.size());
		eventCount = kevent(m_kqueue, &m_changeList[0], m_changeList.size(),
							&m_eventList[0], m_eventList.size(), &timeout);
		m_changeList.clear();
		if (eventCount < 0)
			throw runtime_error("kevent error");

		for (int i = 0; i < eventCount; ++i)
			processEvents(m_eventList[i]);
	}
}
catch (...)
{
}

void
ServerManager::processEvents(struct kevent& event)
{
	EventTarget&	target = m_eventTargetMap[event.ident];

	switch (target.type)
	{
		case EventTarget::SERVER:
			Server*	server = reinterpret_cast<Server*>(target.target);
			server->handleEvent(event);
			break;
		case EventTarget::CLIENT:
			Client*	client = reinterpret_cast<Client*>(target.target);
			client->handleEvent(event);
			break;
		case EventTarget::CGI:
			Cgi*	cgi = reinterpret_cast<Cgi*>(target.target);
			cgi->handleEvent(event);
			break;
		default:
			throw logic_error("invalid EventTarget::target member variable");
	}
}

void
ServerManager::addEventTarget(int fd, EventTarget::e_type type, void* target)
{
	EventTarget	et;

	et.type = type;
	et.target = target;
	m_eventTargetMap[fd] = et;
}

std::ostream&	operator<<(std::ostream& os, const ServerManager& manager)
{
	for (uint32_t i = 0; i < manager.m_serverList.size(); ++i)
		os << manager.m_serverList[i] << '\n';
	return os;
}
