#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>

#include "Server.hpp"
#include "ConfigParser.hpp"

using namespace std;

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
	void			run(const string& path);

// member variables
	vector<Server>	m_serverList;
	ConfigParser	m_configParser;
};

#endif
