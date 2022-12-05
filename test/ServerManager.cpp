#include <sys/select.h>

#include <netinet/in.h>
#include <fcntl.h>

#include "ServerManager.hpp"

#define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_len*/		INET_ADDRSTRLEN,\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){htonl(addr)},\
/*sin_zero*/	{0, }\
})

// constructors & destructor
ServerManager::ServerManager()
{
}

ServerManager::~ServerManager()
{
}

void	ServerManager::IOmultiplex()
{
	struct timeval	timeInterval = {0, };

	while (1)
	{
		timeInterval = (struct timeval){0, };
		if (select(s_maxFd + 1, &readSet, &writeSet, NULL, &timeInterval) < 0)
		{
			std::cout << "select error\n";
			continue;
		}
		for (int i = 0; i < serverVec.size(); ++i)
		{
			MaxFd::tasMaxFd(serverVec[i].acceptConnection(&readSet));
			m_serverVec[i].readOperation(&readSet);
			m_serverVec[i].writeOperation(&writeSet);
		}
	}
}

void	ServerManager::addNewServer()
{
	int	listenSocket = getListenSocket();

	m_serverVec.push_back(Server(listenSocket));
}

void	ServerManager::addServer(Server const& server)
{
	m_serverVec.push_back(server);
}

int	ServerManager::getListenSocket()
{
	int			passiveSocketFd;
	sockaddr_in	socketAddr = {0, };
	in_addr		inetAddr = {0, };

	if ((passiveSocketFd = socket(PF_INET, SOCK_STREAM, 0) == -1))
		throw std::runtime_error("socket error");

	socketAddr = GET_SOCKADDR_IN(INADDR_ANY, 6666);
	if (fcntl(passiveSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("socket error");

	if (bind(passiveSocketFd, reinterpret_cast<sockaddr*>(&socketAddr), sizeof(socketAddr)) == -1
	 || listen(passiveSocketFd, 16) == -1)
		throw std::runtime_error("socket error");

	return passiveSocketFd;
}

#undef GET_SOCKADDR_IN
