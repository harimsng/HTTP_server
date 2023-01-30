#include <iostream>

#include "Config.hpp"
#include "util/Util.hpp"
#include "VirtualServer.hpp"

using namespace	std;

VirtualServer::VirtualServer()
{
	setToDefault();
}

VirtualServer::~VirtualServer()
{
}

void
VirtualServer::setToDefault()
{
	// m_index;
	m_serverNames = vector<string>(1, "");
//	m_errorCode;
//	m_root;
//	m_errorPages;
	m_listen = GET_SOCKADDR_IN(listenIp, listenPort);
	m_clientMaxBodySize = 1 << 13; // 8kb
//	m_uriBufferSize;
//
	m_addrKey = Util::convertAddrKey(listenIp, listenPort);
}

std::ostream&
operator<<(std::ostream& os, const VirtualServer& server)
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
	// os << "\tindex " << server.m_index << '\n';
	os << "\tclient_max_body_size " << server.m_clientMaxBodySize << '\n';
	for (map<string, Location>::const_iterator itr = server.m_locationTable.begin();
			itr != server.m_locationTable.end();
			itr++)
	{
		os << itr->second;
	}
	// for (uint32_t i = 0; i < server.m_locationList.size(); ++i)
	//     os << server.m_locationList[i];
	os << "}\n";
	return os;
}
