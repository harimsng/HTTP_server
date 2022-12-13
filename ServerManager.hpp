#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>

#include "Server.hpp"
#include "parser/config_parser/ConfigParser.hpp"

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
	ConfigParser		m_configParser;

	friend std::ostream&	operator<<(std::ostream& os, const ServerManager& manager);
};

#endif
