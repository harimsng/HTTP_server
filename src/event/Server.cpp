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
Server::Server(uint64_t addrKey)
:	m_socket(Socket<Tcp>())

{
	m_fd = m_socket.m_fd;
	m_socket.m_addr =
		GET_SOCKADDR_IN(addrKey & 0xffffffff, addrKey >> 32);
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
Server::initServer()
{
	if (m_socket.m_fd < 0)
		throw std::runtime_error("server socket() error");

	if (m_socket.bind(&m_socket.m_addr) < 0)
	{
		close(m_socket.m_fd);
		throw std::runtime_error("server socket bind() error");
	}
	if (m_socket.listen() < 0)
	{
		close(m_socket.m_fd);
		throw std::runtime_error("server socket listen() error");
	}
	LOG(DEBUG, "listen server binds and listen on : %s", Util::getFormattedAddress(m_socket.m_addr).data());
}

Server::IoEventPoller::EventStatus
Server::handleEventWork()
{
	struct sockaddr clientSocketAddr;
	socklen_t clientSocketAddrLen = sizeof(struct sockaddr);
	uint32_t addr;
	uint16_t port;

	switch (m_filter)
	{
		case IoEventPoller::READ:
			int		clientFd;
			clientFd = m_socket.accept();

			if (clientFd < 0)
				throw std::runtime_error("accept error in Server::handleEvent()");

			getsockname(clientFd, &clientSocketAddr, &clientSocketAddrLen);
			addr = (*(sockaddr_in*)(&clientSocketAddr)).sin_addr.s_addr;
			port = (*(sockaddr_in*)(&clientSocketAddr)).sin_port;
			LOG(DEBUG, "read event to server toward %s", Util::getFormattedAddress(ntohl(addr), htons(port)).data());

			Client* client;
			client = new Client(clientFd);
			ServerManager::registerEvent(clientFd, IoEventPoller::ADD,
					IoEventPoller::READ, client);
			break;
		default:
			throw std::runtime_error("not handled event filter in Server::handleEvent()");
	}
	return IoEventPoller::NORMAL;
}
