#include <netinet/in.h>

#include <vector>
#include <iostream>

#include "ServerManager.hpp"

static int	getListenSocket();

struct	MaxFd
{
	static int	s_maxFd;
	static int	tasMaxFd(int fd)
	{
		if (fd > s_maxFd)
			s_maxFd = fd;
		return fd;
	};
};

int	main()
{
	ServerManager	serverManager;
	
	serverManager.addNewServer();
	FD_ZERO(&readSet);
	FD_SET(listenSocketFd, &readSet);
	std::cout << "server starts\n";
	serverManager.IOmultiplex();
}
