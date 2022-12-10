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
}
catch (...)
{
}
