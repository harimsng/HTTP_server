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

# ifdef __APPLE__
#define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_len*/		INET_ADDRSTRLEN,\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){(in_addr_t)htonl(addr)},\
/*sin_zero*/	{0, }\
})
# endif
# ifdef __linux__
# define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){htonl(addr)},\
/*sin_zero*/	{0, }\
})
# endif
#endif
