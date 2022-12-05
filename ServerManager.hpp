#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>

#include "Server.hpp"

class	ServerManager
{
public:
// constructors & destructor
	ServerManager();
	~ServerManager();
	ServerManager(ServerManager const& serverManager);

// operators
	ServerManager	&operator=(ServerManager const& serverManager);

// member functions
	void	run();

// member variables
	std::vector<Server>	m_serverList;
};

#endif
