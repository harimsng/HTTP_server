#include "OsDependency.hpp"
#include "ServerManager.hpp"
#include "VirtualServer.hpp"
#include "event/Client.hpp"
#include "event/Server.hpp"

#include <iostream>

using namespace std;
// deleted
Server&
Server::operator=(const Server& server)
{
	(void)server;
	throw logic_error("Server::operator=() is forbidden");
	return *this;
}

// constructors & destructor
Server::Server()
:	m_socket(Socket<Tcp>())
{
	m_fd = m_socket.m_fd;
}

Server::~Server()
{
}

Server::Server(const Server& server)
:	m_socket(server.m_socket)
{
	m_fd = m_socket.m_fd;
}

void
Server::initServer(uint32_t addr, uint16_t port)
{
	if (m_socket.m_fd < 0)
		throw std::runtime_error("server socket() error");

	Tcp::SocketAddr	sockaddr = GET_SOCKADDR_IN(addr, port);
	if (m_socket.bind(&sockaddr) < 0)
	{
		close(m_socket.m_fd);
		throw std::runtime_error("server socket bind() error");
	}
	if (m_socket.listen() < 0)
	{
		close(m_socket.m_fd);
		throw std::runtime_error("server socket listen() error");
	}
	LOG(DEBUG, "listen server binds and listen on : %s", Util::getFormattedAddress(m_socket.m_fd).data());
}

Server::IoEventPoller::EventStatus
Server::handleEventWork()
{
	switch (m_filter)
	{
		case IoEventPoller::READ:
			int		clientFd;
			clientFd = m_socket.accept();

			if (clientFd < 0)
				throw std::runtime_error("accept error in Server::handleEvent()");

			Client* client;
			client = new Client(clientFd);
			LOG(DEBUG, "read event to server toward %s", Util::getFormattedAddress(clientFd).data());
			ServerManager::registerEvent(clientFd, IoEventPoller::ADD,
					IoEventPoller::READ, client);
			break;
		default:
			throw std::runtime_error("not handled event filter in Server::handleEvent()");
	}
	return IoEventPoller::NORMAL;
}
