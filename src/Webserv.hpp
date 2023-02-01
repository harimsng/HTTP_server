#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "OsDependency.hpp"
#include "util/AddrKey.hpp"

#include <map>
#include <string>

class	Server;
class	VirtualServer;

typedef	std::map<std::string, VirtualServer*>	ServerNameTable;
typedef	std::map<AddrKey, ServerNameTable>		VirtualServerTable;
typedef std::map<AddrKey, Server*>				ListenServerTable;

#endif
