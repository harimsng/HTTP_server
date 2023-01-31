#include "Webserv.hpp"
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
	RequestHandler::setMethodConvertTable();

	configParser.init(path, s_virtualServerTable);
	configParser.parse();
	Logger::log(Logger::INFO, "finished parsing configuration file");
}

void
ServerManager::initServers() try
{
	t_uint64	prevPort = 0;
	bool		anyAddrFlag = false;

	for (VirtualServerTable::iterator itr = s_virtualServerTable.begin();
		 itr != s_virtualServerTable.end();
		 ++itr)
	{
		AddrKey					addrKey = itr->first;
		Socket<Tcp>::SocketAddr	sockAddr = addrKey.splitAddrKey();
		t_uint32	addr = sockAddr.sin_addr.s_addr;
		t_uint16	port = sockAddr.sin_port;

		if (anyAddrFlag == true && prevPort == port)
			continue;

		anyAddrFlag = addr == 0;

		Server*	newServer = new Server();

		newServer->initServer(addr, port);
		registerEvent(newServer->m_fd, IoEventPoller::ADD, IoEventPoller::READ, newServer);
		s_listenServerTable[addrKey] = newServer;
		prevPort = port;
	}

	Logger::log(Logger::INFO, "%zu listen servers are initiated\n", s_listenServerTable.size());
}
catch (std::exception& e)
{
	for (ListenServerTable::iterator itr = s_listenServerTable.begin();
		 itr != s_listenServerTable.end();
		 ++itr)
	{
		close(itr->second->m_fd);
	}
	throw;
}

void
ServerManager::run() try
{
	initServers();
	RequestHandler::initExtensionList();

	while (1)
	{
		s_ioEventPoller.poll();
	}
	Logger::log(Logger::INFO, "%zu listen servers exited\n", s_listenServerTable.size());
}
// TODO: cleanup
catch (std::runtime_error& e)
{
	LOG(ERROR, "%s", e.what());
}
catch (HttpErrorHandler& e)
{
	LOG(ERROR, "%s", e.getErrorMessage().data());
}
catch (...)
{
	LOG(ERROR, "unexpected error");
}

void
ServerManager::registerEvent(int fd, IoEventPoller::e_operation op,
			int filter, EventObject* object)
{
	s_ioEventPoller.add(fd, op, static_cast<IoEventPoller::e_filters>(filter), object);
}
