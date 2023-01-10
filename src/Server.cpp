#include "ServerManager.hpp"
#include "Client.hpp"
#include "Server.hpp"

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
:	m_socket(Socket<Tcp>()),
	m_fd(m_socket.m_fd)
{
}

Server::~Server()
{
}

Server::Server(const Server& server)
:	m_socket(server.m_socket),
	m_fd(server.m_fd)
{
}

void
Server::initServer()
{
	Socket<Tcp>::SocketAddr socketaddr;
	
	if (m_socket.m_fd < 0)
		throw std::runtime_error("server socket() error");

	socketaddr.sin_port = m_port;
	if (m_socket.bind(&socketaddr) < 0)
		throw std::runtime_error("server socket bind() error");
	if (m_socket.listen() < 0)
		throw std::runtime_error("server socket listen() error");
}

Server::IoEventPoller::EventStatus
Server::handleEvent(const IoEventPoller::Event& event)
{
	IoEventPoller::e_filters	filter = event.getFilter();

	switch (filter)
	{
		case IoEventPoller::READ:
			int		clientFd;
			clientFd = m_socket.accept();

			if (clientFd < 0)
				throw std::runtime_error("accept error in Server::handleEvent()");

			Client* client;
			client = new Client(clientFd);
			ServerManager<IoEventPoller>::registerEvent(clientFd, IoEventPoller::ADD,
					filter, client);
			// ServerManager<IoEventPoller>::registerEvent(clientFd,
			//         IoEventPoller::ADD, IoEventPoller::READWRITE);
			break;
		default:
			throw std::runtime_error("not handled event filter in Server::handleEvent()");
	}
	return IoEventPoller::NORMAL;
}
