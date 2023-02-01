#include <sstream>

#include "Socket.hpp"

using namespace	std;

template <>
string
Socket<Tcp>::getFormattedAddress(uint32_t addr, uint16_t port)
{
	stringstream	ss;
	uint32_t		bitshift = 24;
	int64_t			bitmask = 0xff000000;

	for (; bitshift > 0; bitmask >>= 8, bitshift -= 8)
	{
		ss << ((addr & bitmask) >> bitshift) << '.';
	}
	ss << (addr & bitmask);
	if (port != 0)
		ss << ':' << port;
	return ss.str();
}

template <>
string
Socket<Tcp>::getFormattedAddress(sockaddr_in& addr)
{
	return getFormattedAddress(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
}

template <>
string
Socket<Tcp>::getFormattedAddress(int fd)
{
	sockaddr_in		addr;
	socklen_t		addrLen = Tcp::socketAddrLen;

	getsockname(fd, reinterpret_cast<sockaddr*>(&addr), &addrLen);
	return getFormattedAddress(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));
}
