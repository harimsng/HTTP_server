#ifndef OSDEPENDENCY_HPP
#define OSDEPENDENCY_HPP

// May need header files for socket?

# ifdef __APPLE__

#  define IO_EVENT_POLLER Kqueue
#  include "io/Kqueue.hpp"
// reason for not directly including the header file is to minimize compile time overhead.

#  define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_len*/		INET_ADDRSTRLEN,\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){(in_addr_t)htonl(addr)},\
/*sin_zero*/	{0, }\
})
# endif
# ifdef __linux__
#  define IO_EVENT_POLLER Epoll
#  include "io/Epoll.hpp"

#  define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){htonl(addr)},\
/*sin_zero*/	{0, }\
})
# endif

#endif
