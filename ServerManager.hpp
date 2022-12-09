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
	void	parse(const char* path);
	void	run();

private:
// member variables
	std::vector<Server>	m_serverList;
};

#endif
