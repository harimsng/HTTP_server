#include <ostream>

#include "Server.hpp"

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

Server&	Server::operator=(const Server& server)
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
	os << "server\n{\n";
	os << "\tserver_name " << server.m_serverNames << '\n';

	uint32_t	addr = ntohl(server.m_listen.sin_addr.s_addr);
	os << "\tlisten " << ((addr & 0xff000000) >> 24) << '.' << ((addr & 0xff0000) >> 16) << '.'
		<< ((addr & 0xff00) >> 8) << '.' << (addr & 0xff) << ':' << ntohs(server.m_listen.sin_port) << '\n';
	os << "\tindex " << server.m_index << '\n';
	os << "\tclient_max_body_size " << server.m_clientMaxBodySize << '\n';
	for (uint32_t i = 0; i < server.m_locationList.size(); ++i)
		os << server.m_locationList[i];
	os << "}\n";
	return os;
}
