#include <iostream>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "parser/config_parser/ConfigParser.hpp"
#include "parser/config_parser/ServerParser.hpp"
#include "parser/config_parser/LocationParser.hpp"
#include "socket/ServerSocket.hpp"

using namespace std;

struct	ServerManager::EventTarget
{
	enum	e_type
	{
		SERVER = 1,
		CLIENT = 2,
		CGI = 3
	};

	e_type	type;
	void*	target;
};

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
	Logger::log(Logger::INFO, "parse complete");
}
catch (std::exception& e)
{
	cout << e.what() << '\n';
}

void
ServerManager::run() try
{
	static const timespec	timeout = {0, 100};
	int						eventCount;

	for (size_t i = 0; i < m_serverList.size(); i++)
		m_serverList[i].initServer();

	m_kqueue = kqueue();
	m_eventList.reserve(1 << 13);
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
ServerManager::handleClientEvent(const struct kevent& event)
{
	int16_t			eventFilter = event.filter;
	ClientSocket&	clientSocket = m_clientSocketMap[event.ident];

	switch (eventFilter)
	{
		case EVFILT_READ:
			clientSocket.readSocket();
			break;
		case EVFILT_READ:
			clientSocket.writeSocket();
			break;
		default:
			throw runtime_error("invalid kevent filter");
	}
}

void
ServerManager::handleServerEvent(const struct kevent& event)
{
	int16_t	eventFilter = event.filter;
	Server&	server = m_serverMap[event.ident];

	switch (eventFilter)
	{
		case EVFILT_READ:
			server.
			break;
		default:
			throw runtime_error("invalid kevent filter");
}

std::ostream&	operator<<(std::ostream& os, const ServerManager& manager)
{
	for (uint32_t i = 0; i < manager.m_serverList.size(); ++i)
		os << manager.m_serverList[i] << '\n';
	return os;
}
