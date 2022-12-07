#include "ServerManager.hpp"

using namespace std;

// constructors & destructor
ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
}

void
ServerManager::run(const string& path)
{
	m_configParser.init(path);
	m_configParser.parse(m_serverList);
	for (size_t i = 0; i < m_serverList.size(); i++)
		m_serverList[i].initServer();
	while (1)
	{
		for (size_t i = 0; i < m_serverList.size(); i++)
			m_serverList[i].run();
	}
}
