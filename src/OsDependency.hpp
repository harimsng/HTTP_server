#ifndef OSDEPENDENCY_HPP
#define OSDEPENDENCY_HPP

// May need header files for socket?

# ifdef __APPLE__

#  define IO_EVENT_POLLER Kqueue
#  define IO_EVENT_HEADER "io/Kqueue.hpp"
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
#  define IO_EVENT_HEADER "io/Epoll.hpp"
#  include <stdint.h>

#  define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){htonl(addr)},\
/*sin_zero*/	{0, }\
})
# endif

#endif
