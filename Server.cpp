#include "Server.hpp"
#include "socket/ClientSocket.hpp"

using namespace std;

// constructors & destructor
Server::Server()
{
}

Server::~Server()
{
}

void
Server::initServer()
{
	m_kq = kqueue();
	if (m_kq < 0)
	{
		// throw exception;
	}
	m_serverSocket.createSocket(*this);
	addEvents(m_serverSocket.m_SocketFd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
}

void
Server::run()
{
	int newEventCnt;

	newEventCnt = waitEvent();
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

	newEvents = kevent(m_kq, &m_changeList[0], m_changeList.size(),
			m_eventList, EVENT_SIZE, NULL);
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

		clientSocket.createSocket(m_serverSocket);
		m_clientSocket.insert(make_pair(clientSocket.m_SocketFd, clientSocket));
		addEvents(clientSocket.m_SocketFd, EVFILT_READ,
				EV_ADD | EV_ENABLE, 0, 0, NULL);
		return (1);
	}
	else
	{
		// request class가 ClientSocket의 fd를 가지고와서 read 실행?
		//
		
	}
	return (0);
}

int
Server::writeEventHandler(struct kevent* curEvent)
{
	(void)curEvent;
	return (0);
}
