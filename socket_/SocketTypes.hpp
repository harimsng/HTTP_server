#ifndef SOCKETTYPES_HPP
#define SOCKETTYPES_HPP

#include <sys/socket.h>

#include <netinet/in.h>
struct	Tcp
{
	typedef sockaddr_in	socketAddr;

	static const socklen_t	socketAddrLen = INET_ADDRSTRLEN;
	static const int		domain = AF_INET;
	static const int		type = SOCK_STREAM;
	static const int		protocol = 0;
};

#endif
