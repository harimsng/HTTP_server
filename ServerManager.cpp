#include <iostream>

#include "ServerManager.hpp"
#include "ConfigParser.hpp"
#include "ServerParser.hpp"
#include "LocationParser.hpp"

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
}
catch (std::exception& e)
{
	cout << e.what() << '\n';
}

void
ServerManager::run() try
{
	for (size_t i = 0; i < m_serverList.size(); i++)
		m_serverList[i].initServer();
	while (1)
	{
		for (size_t i = 0; i < m_serverList.size(); i++)
			m_serverList[i].run();
	}
}
catch (...)
{
}
