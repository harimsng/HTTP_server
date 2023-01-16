#include "ServerManager.hpp"

// static member definitions
ServerManager::IoEventPoller	ServerManager::s_ioEventPoller;

VirtualServerTable	ServerManager::s_virtualServerTable;

ListenServerTable	ServerManager::s_listenServerTable;


// member function definitions
ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
}

void
ServerManager::parseConfig(const char* path)
{
	ConfigParser	configParser;

	configParser.init(path, s_virtualServerTable);
	configParser.parse();
	Logger::log(Logger::INFO, "parsing configuration file finished");
}

void
ServerManager::initServers()
{
	std::set<uint64_t>	portSet;

	for (VirtualServerTable::iterator itr = s_virtualServerTable.begin();
		 itr != s_virtualServerTable.end();
		 ++itr)
	{
		portSet.insert(itr->first & 0xffff);
	}

	for (std::set<uint64_t>::iterator itr = portSet.begin();
		 itr != portSet.end();
		 ++itr)
	{
		Server*	newServer = new Server(*itr);

		newServer->initServer();
		s_ioEventPoller.add(newServer->m_fd, IoEventPoller::ADD,
				IoEventPoller::READ, newServer);
		s_listenServerTable[*itr] = newServer;
	}

	Logger::log(Logger::INFO, "%zu listen servers are initiated", s_listenServerTable.size());
}

void
ServerManager::run() try
{
	initServers();

	while (1)
	{
		s_ioEventPoller.poll();
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

void
ServerManager::registerEvent(int fd, IoEventPoller::e_operation op,
			IoEventPoller::e_filters filter, EventObject* object)
{
	s_ioEventPoller.add(fd, op, filter, object);
}
