#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <map>
#include <vector>
#include <string>

class	Server;

typedef	std::map<std::string, Server*>		ServerNameTable;
typedef	std::map<uint64_t, ServerNameTable>	VirtualServerTable;
/*

listen 80			-> 80 << 32 + 0 == 80

listen 1.2.3.4:80	-> 80 << 32 + convert(1, 2, 3, 4) != 80

listen 4.3.2.1:100 	-> 100 << 32 + convert(4, 3, 2, 1) != 100

listen 5.4.3.2:100	-> 100 << 32 + convert(5, 4, 3, 2) != 100

set<port>	portTable;

for (DefaultServerTable::iterator start = table.begin(); start != table.end(); ++start;)
{
	portTable.insert(start->first & 0xff);
}

for (auto port: portTable)
{
	Server*	newServer = new Server();

	newServer.fd = socket();
	bind(newServer.fd, {0,0,0,0}, port);
	addEventObject(Server, ,newServer);
}

fd = accept(serverFd);
getsockname(fd, &ip, &port);

if (VirtualServerTable.count(ip << 16 | port) == 0)
{
	if (VirtualServerTable.count(port) == 0)
		throw
	else
		Client.serverTable = VirtualServerTable[port];
}
ip << 16 | port

for (DefaultServerTable::iterator start = table.begin(); start != table.end(); ++start;)
{
	uint64_t	key = start->first;

	if (key == (key & 0xff00000000))
	{
		ListenServerTable.insert();
		while (start->first & 0xff00000000 == key)
		{
		}
		continue;
	}

}
*/

#endif
