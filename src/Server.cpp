#include "Server.hpp"

Server::Server()
:	m_fd(m_socket.m_fd)
{
	setToDefault();
}

Server::~Server()
{
}

Server::Server(Server const& server)
:	m_socket(server.m_socket),
	m_fd(m_socket.m_fd)
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

uint64_t
Server::getAddrKey() const
{
	return m_addrKey;
}

#define DEFAULT_NAME ("") // coule be hostname
#define DEFAULT_ADDR (INADDR_ANY)
#define DEFAULT_PORT (8000)
// TODO: fill up default variable list

void
Server::setToDefault()
{
//	m_index;
	m_serverNames = vector<string>(1, "");
//	m_errorCode;
//	m_root;
//	m_errorPages;
	m_listen = GET_SOCKADDR_IN(DEFAULT_ADDR, DEFAULT_PORT);
	m_clientMaxBodySize = 1 << 13; // 8kb
//	m_uriBufferSize;
}

std::ostream&
operator<<(std::ostream& os, const Server& server)
{
	uint32_t	addr = ntohl(server.m_listen.sin_addr.s_addr);

	os << "server\n{\n";
	os << "\tserver_name    ";
	for (size_t i = 0; i < server.m_serverNames.size(); ++i)
		os << server.m_serverNames[i] << ' ';
	os << '\n';
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
