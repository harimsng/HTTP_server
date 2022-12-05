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
}
