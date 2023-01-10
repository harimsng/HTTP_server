#ifndef SOCKETTYPES_HPP
#define SOCKETTYPES_HPP

#include <sys/socket.h>
#include <netinet/in.h>

#include "OsDependency.hpp"

struct	Tcp
{
	struct	SocketAddr: public sockaddr_in
	{
		SocketAddr()
		{
			sin_len = socketAddrLen;
			sin_family = domain;
			sin_port = 0;
			sin_addr.s_addr = static_cast<in_addr_t>(0);
			::memset(sin_zero, 0, sizeof(sin_zero));
		};
	};

	static const socklen_t	socketAddrLen = INET_ADDRSTRLEN;
	static const int		domain = AF_INET;
	static const int		type = SOCK_STREAM;
	static const int		protocol = 0;
};

#endif
