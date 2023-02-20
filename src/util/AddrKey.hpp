#ifndef ADDRKEY_HPP
#define ADDRKEY_HPP

#include "Types.hpp"
#include "socket/Socket.hpp"
#include "socket/Tcp.hpp"

struct	AddrKey
{
	void
	setAddrKey(t_uint32 addr, t_uint16 port)
	{
		value = (static_cast<t_uint64>(port) << 32) + addr;
	}

	Socket<Tcp>::SocketAddr
	splitAddrKey()
	{
		Socket<Tcp>::SocketAddr	sockAddr;
		sockAddr.sin_addr.s_addr = value & 0xffffffff;
		sockAddr.sin_port = value >> 32;
		return sockAddr;
	}
	t_uint64	value;

	bool	operator<(const AddrKey& rhs) const
	{
		return value < rhs.value;
	}

	bool	operator==(const AddrKey& rhs) const
	{
		return value == rhs.value;
	}

	bool	operator>(const AddrKey& rhs) const
	{
		return !(*this < rhs) && !(*this == rhs);
	}

	bool	operator>=(const AddrKey& rhs) const
	{
		return !(*this < rhs);
	}

	bool	operator<=(const AddrKey& rhs) const
	{
		return !(*this > rhs);
	}
};

#endif
