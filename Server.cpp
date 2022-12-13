#include <sys/event.h>

#include <iostream>

#include "Logger.hpp"
#include "Server.hpp"
#include "socket/ClientSocket.hpp"

using namespace std;

// constructors & destructor
Server::Server()
{
	setToDefault();
}

Server::~Server()
{
}
Server::Server(Server const& server)
{
	*this = server;
}

Server&
Server::operator=(const Server& server)
{
	m_index = server.m_index;
	m_serverNames = server.m_serverNames;
	m_errorCode = server.m_errorCode;
	m_root = server.m_root;
	m_errorPages = server.m_errorPages;
	m_listen = server.m_listen;
	m_clientMaxBodySize = server.m_clientMaxBodySize;
	m_uriBufferSize = server.m_uriBufferSize;
	m_locationList = server.m_locationList;
	return *this;
}

void	Server::setToDefault()
{
//	m_index;
	m_serverNames = "";
//	m_errorCode;
//	m_root;
//	m_errorPages;
	m_listen = GET_SOCKADDR_IN(INADDR_ANY, 8000);
	m_clientMaxBodySize = 1 << 13; // 8kb
//	m_uriBufferSize;
}

std::ostream&	operator<<(std::ostream& os, const Server& server)
{
	uint32_t	addr = ntohl(server.m_listen.sin_addr.s_addr);

	os << "server\n{\n";
	os << "\tserver_name " << server.m_serverNames << '\n';
	os << "\tlisten " << ((addr & 0xff000000) >> 24) << '.'
	   << ((addr & 0xff0000) >> 16) << '.'
	   << ((addr & 0xff00) >> 8) << '.'
	   << (addr & 0xff) << ':'
	   << ntohs(server.m_listen.sin_port) << '\n';
	os << "\tindex " << server.m_index << '\n';
	os << "\tclient_max_body_size " << server.m_clientMaxBodySize << '\n';
	for (uint32_t i = 0; i < server.m_locationList.size(); ++i)
		os << server.m_locationList[i];
	os << "}\n";
	return os;
}

void
Server::initServer()
{
	m_kq = kqueue();
	if (m_kq < 0)
	{
		// throw exception;
	}
	m_serverSocket.createSocket(m_listen);
	addEvents(m_serverSocket.m_SocketFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

void
Server::run()
{
	int newEventCnt;

	newEventCnt = waitEvent();
	if (newEventCnt == -1)
		return;
	eventSocket(newEventCnt);
}

void
Server::addEvents(uintptr_t ident, int16_t filter, uint16_t flags,
		uint32_t fflags, intptr_t data, void* udata)
{
	struct kevent tempEvent;

	EV_SET(&tempEvent, ident, filter, flags, fflags, data, udata);
	m_changeList.push_back(tempEvent);
}

int
Server::waitEvent()
{
	int newEvents;
	static const struct timespec tout = { 1, 0 };

	newEvents = kevent(m_kq, &m_changeList[0], m_changeList.size(),
			m_eventList, EVENT_SIZE, &tout);
	Logger::log(Logger::INFO, "new event occured");
	if (newEvents == -1)
	{
		// throw exception;
	}
	m_changeList.clear();
	return (newEvents);
}

void
Server::eventSocket(int newEventCnt)
{
	for (int i = 0; i < newEventCnt; i++)
	{
		struct kevent* curEvent;

		curEvent = &m_eventList[i];
		if (curEvent->flags & EV_ERROR)
		{
			if (m_serverSocket.m_SocketFd == (int)curEvent->ident)
			{
				// throw exception("server socket fail");
			}
			else
			{
				// throw exception("client socket fail");
			}
		}
		else if (curEvent->filter == EVFILT_READ)
		{
			if (readEventHandler(curEvent))
					return ;
		}
		else if (curEvent->filter == EVFILT_WRITE)
			writeEventHandler(curEvent);
	}
}

int
Server::readEventHandler(struct kevent* curEvent)
{
	if (m_serverSocket.m_SocketFd == (int)curEvent->ident)
	{
		ClientSocket clientSocket;

		Logger::log(Logger::INFO, "server socket read");
		clientSocket.createSocket(m_serverSocket);
		m_clientSocket.insert(make_pair(clientSocket.m_SocketFd, clientSocket));
		addEvents(clientSocket.m_SocketFd, EVFILT_READ,
				EV_ADD | EV_ENABLE, 0, 0, NULL);
		// addEvents(clientSocket.m_SocketFd, EVFILT_WRITE,
		//         EV_ADD | EV_ENABLE, 0, 0, NULL);
		return (1);
	}
	else
	{
		ClientSocket& clientSocket = m_clientSocket.find(curEvent->ident)->second;

		Logger::log(Logger::INFO, "client socket read");
		clientSocket.readSocket(curEvent->data, *this);
	}
	return (0);
}

int
Server::writeEventHandler(struct kevent* curEvent)
{
	(void)curEvent;
	return (0);
}
