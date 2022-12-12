#ifndef SERVER_HPP
#define SERVER_HPP

// for gnu c portability and c++98 standard. should be removed later
#include <stdint.h>
#include <sys/event.h>

#include <map>
#include <vector>
#include <string>
#include <utility>

#include "Location.hpp"
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "Communicator.hpp"

#define EVENT_SIZE 8

class	Server
{
	friend class	ServerParser;
	friend class	ServerSocket;

// deleted
	Server& operator=(const Server& copy);

public:
// constructors & destructor
	Server();
	Server(const Server& copy) {(void)copy;};
	~Server();

// member functions
	void	initServer();
	void	run();

private:
// member functions - kqueue
	void	addEvents(uintptr_t ident, int16_t filter, uint16_t flags,
			uint32_t fflags, intptr_t data, void* udata);
	int		waitEvent();
	void	eventSocket(int newEventCnt);
	int		readEventHandler(struct kevent* curEvent);
	int		writeEventHandler(struct kevent* curEvent);

// member variales - config
	std::string					m_index;
	std::string					m_serverName;
	std::string					m_errorCode;
	std::string					m_root;
	std::string					m_errorPath;
	int32_t						m_listen;
	int32_t						m_clientMaxBodySize;
	int32_t						m_uriBufferSize;
	std::vector<Location>		m_locationList;

// member variables - kevent
	std::vector<struct kevent>	m_changeList;
	struct kevent				m_eventList[EVENT_SIZE];
	int							m_kq;

// member variables - socket
	std::map<int, ClientSocket>	m_clientSocket;
	ServerSocket				m_serverSocket;
};

#endif
