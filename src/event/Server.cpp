#include "OsDependency.hpp"
#include "ServerManager.hpp"
#include "event/Client.hpp"
#include "event/Server.hpp"

// deleted
Server&
Server::operator=(const Server& server)
{
	(void)server;
	throw logic_error("Server::operator=() is forbidden");
	return *this;
}

// constructors & destructor
Server::Server(uint16_t port)
:	m_socket(Socket<Tcp>()),
	m_port(port)
{
	m_fd = m_socket.m_fd;
}

Server::~Server()
{
}

Server::Server(const Server& server)
:	m_socket(server.m_socket),
	m_port(server.m_port)
{
	m_fd = m_socket.m_fd;
}

void
Server::initServer()
{
	if (m_socket.m_fd < 0)
		throw std::runtime_error("server socket() error");

	Socket<Tcp>::SocketAddr socketaddr = GET_SOCKADDR_IN(0, m_port);
	if (m_socket.bind(&socketaddr) < 0)
		throw std::runtime_error("server socket bind() error");
	if (m_socket.listen() < 0)
		throw std::runtime_error("server socket listen() error");
	LOG(DEBUG, "listen server binds and listen on port: %u", m_port);
}

Server::IoEventPoller::EventStatus
Server::handleEventWork()
{
	switch (m_filter)
	{
		case IoEventPoller::READ:
			LOG(DEBUG, "read event to server");
			int		clientFd;
			clientFd = m_socket.accept();

			if (clientFd < 0)
				throw std::runtime_error("accept error in Server::handleEvent()");

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
