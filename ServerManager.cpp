#include <iostream>

#include "Logger.hpp"
#include "ServerManager.hpp"
#include "parser/config_parser/ConfigParser.hpp"
#include "parser/config_parser/ServerParser.hpp"
#include "parser/config_parser/LocationParser.hpp"

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
	Logger::log(Logger::INFO, "parse complete");
}
catch (std::exception& e)
{
	cout << e.what() << '\n';
}

void
ServerManager::run() try
{
	Logger::log(Logger::INFO, "server running");
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

std::ostream&	operator<<(std::ostream& os, const ServerManager& manager)
{
	for (uint32_t i = 0; i < manager.m_serverList.size(); ++i)
		os << manager.m_serverList[i] << '\n';
	return os;
}
