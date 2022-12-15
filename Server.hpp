#ifndef SERVER_HPP
#define SERVER_HPP

// for gnu c portability and c++98 standard. should be removed later
#include <stdint.h>
#include <netinet/in.h>

#include <map>
#include <vector>
#include <string>
#include <utility>

#include "Location.hpp"
#include "socket/ServerSocket.hpp"
#include "socket/ClientSocket.hpp"
#include "communicator/Communicator.hpp"

#define EVENT_SIZE 8

#define GET_SOCKADDR_IN(addr, port) ((sockaddr_in){\
/*sin_len*/		INET_ADDRSTRLEN,\
/*sin_family*/	AF_INET,\
/*sin_port*/	htons(port),\
/*sin_addr*/	(in_addr){htonl(addr)},\
/*sin_zero*/	{0, }\
})

class	Server
{
	friend class	ServerParser;
	friend class	ServerSocket;

public:
// constructors & destructor
	Server();
	~Server();
	Server(Server const& server);
	Server	&operator=(Server const& server);

// member functions
	void	initServer();

private:
	void	handleEvent(struct kevent& event);
	void	addEvents(uintptr_t ident, int16_t filter, uint16_t flags,
			uint32_t fflags, intptr_t data, void* udata);
	int		readEventHandler(struct kevent* curEvent);
	int		writeEventHandler(struct kevent* curEvent);
	void	setToDefault();

// member variales - config
	std::string					m_index;
	std::string					m_serverNames;
	std::string					m_errorCode;
	std::string					m_root;
	std::string					m_errorPath;
	std::string					m_errorPages;
	sockaddr_in					m_listen;
	int32_t						m_clientMaxBodySize;
	int32_t						m_uriBufferSize;
	std::vector<Location>		m_locationList;

// member variables - socket
	ServerSocket				m_serverSocket;

	friend std::ostream&	operator<<(std::ostream& os, const Server& server);
};

#endif
