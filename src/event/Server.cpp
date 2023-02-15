#include "OsDependency.hpp"
#include "Logger.hpp"
#include "ServerManager.hpp"
#include "VirtualServer.hpp"
#include "event/Client.hpp"
#include "event/Server.hpp"
#include "socket/Socket.hpp"
#include "socket/Tcp.hpp"

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
:	EventObject(server),
	m_socket(server.m_socket)
{
}

void
Server::initServer(uint32_t addr, uint16_t port)
{
	if (m_socket.m_fd < 0)
		throw std::runtime_error("server socket() error");

	Tcp::SocketAddr	sockaddr = GET_SOCKADDR_IN(addr, port);
	LOG(DEBUG, "listen server trying to bind and listen on: %s",
			getFormattedAddress(addr, port).data());
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
}

Server::IoEventPoller::EventStatus
Server::handleReadEventWork()
{
	int		clientFd;
	clientFd = m_socket.accept();

	if (clientFd < 0)
		throw std::runtime_error("accept error in Server::handleEvent()");

	Client* client;
	client = new Client(clientFd);
	LOG(DEBUG, "read event to server toward %s", getFormattedAddress(clientFd).data());
	// Epoll
	ServerManager::registerEvent(clientFd, IoEventPoller::OP_ADD,
			IoEventPoller::FILT_READ, client);
	return IoEventPoller::STAT_NORMAL;
}

Server::IoEventPoller::EventStatus
Server::handleWriteEventWork()
{
	throw std::runtime_error("not handled event filter in Server::handleEvent()");
	return IoEventPoller::STAT_ERROR;
}

Server::IoEventPoller::EventStatus
Server::handleErrorEventWork()
{
	throw std::runtime_error("not handled event filter in Server::handleEvent()");
	return IoEventPoller::STAT_ERROR;
}
