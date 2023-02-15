#ifndef SOCKET_UNIX_HPP
#define SOCKET_UNIX_HPP

#include <sys/socket.h>
#include <sys/un.h>

#include <unistd.h>
#include <cstring>
#include <string>

#include "socket/Socket.hpp"

struct	Unix
{
	struct	SocketAddr: public sockaddr_un
	{
		SocketAddr()
		{
			sun_len = 0;
			sun_family = domain;
			::memset(sun_path, 0, sizeof(sun_path));
		}

		SocketAddr(const sockaddr_un& addr)
		{
			sockaddr_un::operator=(addr);
		}
	};

	static int	initSocket(int fd)
	{
		(void)fd;
		return 0;
	}

	static int	removeSocket(int fd, const sockaddr_un& sockAddr)
	{
		(void)fd;
		return ::unlink(sockAddr.sun_path);
	}

	static const socklen_t	socketAddrLen = sizeof(sockaddr_un);
	static const int		domain = AF_LOCAL;
	static const int		type = SOCK_STREAM;
	static const int		protocol = 0;
};

#endif
