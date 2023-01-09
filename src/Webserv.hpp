#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <map>
#include <vector>
#include <string>

class	Server;

typedef	std::map<std::string, Server*>		ServerNameTable;
typedef	std::map<uint64_t, ServerNameTable>	VirtualServerTable;

#endif
