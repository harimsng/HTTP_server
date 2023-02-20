#include "FdManager.hpp"

// constructors & destructor
FdManager::FdManager()
{
	init();
}

FdManager::~FdManager()
{
}

FdManager::FdManager(FdManager const& fdManager)
{
	init();
}

// operators
FdManager&	FdManager::operator=(FdManager const& fdManager)
{
	return *this;
}

// member functions
void	FdManager::init()
{
	FD_ZERO(&m_readSet);
	FD_ZERO(&m_writeSet);
	FD_ZERO(&m_exceptionSet);
}
