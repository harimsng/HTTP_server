#ifndef SOCKETTYPES_HPP
#define SOCKETTYPES_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include "OsDependency.hpp"

struct	Tcp
{
	struct	SocketAddr: public sockaddr_in
	{
		SocketAddr()
		{
			sockaddr_in::operator=(GET_SOCKADDR_IN(0, 0));
			::memset(sin_zero, 0, sizeof(sin_zero));
		};

		SocketAddr(const sockaddr_in& addr)
		{
			sockaddr_in::operator=(addr);
			::memset(sin_zero, 0, sizeof(sin_zero));
		};
	};

	static const socklen_t	socketAddrLen = INET_ADDRSTRLEN;
	static const int		domain = AF_INET;
	static const int		type = SOCK_STREAM;
	static const int		protocol = 0;
};

#endif
