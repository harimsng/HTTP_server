#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"
#include "FdManager.hpp"

class	ServerManager
{
	ServerManager(ServerManager const& servermanager);
	ServerManager	&operator=(ServerManager const& servermanager);

public:
// constructors & destructor
	ServerManager();
	~ServerManager();

// operators

// member functions
	void	IOmultiplex();

	void	addNewServer();
	void	addServer(Server const& server);
	void	deleteServer(int listenSocket);
private:
	int		getListenSocket();

// member variables
	FdManager			m_fdManager;

	std::vector<Server>	m_serverVec;
};

#endif
