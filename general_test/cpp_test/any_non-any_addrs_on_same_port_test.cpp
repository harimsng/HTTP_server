#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>

#include <iostream>

#define CONVERT_IP(a, b, c, d) \
((a << 24) + (b << 16) + (c << 8) + d)

void	sender();
void	duplicate_recipient();
void	recipient(uint32_t listen_addr);

int	main()
{
	int	pid;

	if ((pid = fork()) < 0)
		return 1;

	if (pid == 0)
	{
		sender();
	}
	else
	{
		duplicate_recipient();
		wait(NULL);
	}

	return 0;
}

void	sender()
{
	sockaddr_in		addr = {
		.sin_len = INET_ADDRSTRLEN,
		.sin_family = PF_INET,
		.sin_port = htons(8000),
		.sin_addr = (in_addr){htonl(CONVERT_IP(10, 15, 6, 1))},
		.sin_zero = {0, }
	};
	int		fd = socket(PF_INET, SOCK_STREAM, 0);

	if (fd < 0)
	{
		std::cerr << std::strerror(errno) << ' ' << errno
		<< ": socket() error\n";
		return;
	}
/*	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) < 0)
	{
		std::cerr << std::strerror(errno)
		<< ": fcntl() error\n";
		return;
	}*/

	int	count;
	for (count = 5; count > 0; --count)
	{
		std::cout << "connecting\n";
		if (connect(fd, (sockaddr*)&addr, sizeof(addr)) == 0)
			break;
		sleep(1);
	}

	if (count == 0)
	{
		std::cerr << std::strerror(errno)
		<< ": sender couldn't connect to recipient\n";
		return;
	}
	std::cout << "connected\n";

	char	buffer[] = "hello server!";
	while (1)
	{
		std::cout << "send << " << buffer << '\n';
		write(fd, buffer, sizeof(buffer));
		sleep(2);
	}
}

void	duplicate_recipient()
{
	int	pid;

	pid = fork();
	// one of two recipient must fail to bind because of duplicate address.
	// 0.0.0.0:port vs 127.0.0.1:port
	// former's address include latter's address. so latter's failure is expected.
	// but, if latter's address takes place first, a failure happens to former.
	// addr1:port vs addr2:port
	// sockets with different address with same port is ok. if both of addresss are not 0.0.0.0.
	if (pid == 0)
	{
		sleep(1);
		recipient(INADDR_ANY);
	}
	else
	{
		recipient(CONVERT_IP(10, 15, 6, 1));
		wait(NULL);
	}
}

void	recipient(uint32_t listen_addr)
{
	const int	backlog = 4;
	sockaddr_in		addr = {
		.sin_len = INET_ADDRSTRLEN,
		.sin_family = PF_INET,
		.sin_port = htons(8000),
		.sin_addr = (in_addr){htonl(listen_addr)},
		.sin_zero = {0, }
	};
	int		fd = socket(PF_INET, SOCK_STREAM, 0);

	if (fd < 0)
	{
		std::cerr << std::strerror(errno) << ' ' << errno
		<< ": socket() error\n";
		return;
	}

	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) < 0)
	{
		std::cerr << std::strerror(errno) << ' ' << errno
		<< ": fcntl() error\n";
		return;
	}

	sockaddr_in		sender_addr;
	socklen_t		sender_addrlen;

	if (bind(fd, (sockaddr*)&addr, sizeof(addr)) < 0)
	{
		std::cerr << std::strerror(errno) << ' ' << errno
		<< ": bind() error\n";
		uint32_t	addr_int = ntohl(addr.sin_addr.s_addr);
		uint32_t	port_int = ntohs(addr.sin_port);
		std::cerr << "recipient information ";
		std::cerr << (addr_int >> 24) << '.'
				<< ((addr_int & 0xff0000) >> 16) << '.'
				<< ((addr_int & 0xff00) >> 8) << '.'
				<< (addr_int & 0xff) << ':'
				<< port_int << '\n';
		return;
	}

	if (listen(fd, backlog) < 0)
	{
		std::cerr << std::strerror(errno) << ' ' << errno
		<< ": listen() error\n";
		return;
	}

	int	sender_fd;
	for (int count = 5; count > 0; --count)
	{
		std::cout << "accepting\n";
		sender_fd = accept(fd, (sockaddr*)&sender_addr, &sender_addrlen);
		if (sender_fd > -1)
			break;
		sleep(1);
	}

	if (sender_fd < 0)
	{
		std::cerr << std::strerror(errno)
		<< ": recipient couldn't accept sender's connection\n";
		return;
	}

	if (fcntl(sender_fd, F_SETFL, fcntl(sender_fd, F_GETFL, 0) & ~O_NONBLOCK) < 0)
	{
		std::cerr << std::strerror(errno) << ' ' << errno
		<< ": fcntl() error\n";
		return;
	}

	std::cout << "accepted\n";
	{
	getsockname(fd, (sockaddr*)&sender_addr, &sender_addrlen);
	uint32_t	addr_int = ntohl(sender_addr.sin_addr.s_addr);
	uint32_t	port_int = ntohs(sender_addr.sin_port);
	std::cout << "accepted recipient information ";
	std::cout << (addr_int >> 24) << '.'
			<< ((addr_int & 0xff0000) >> 16) << '.'
			<< ((addr_int & 0xff00) >> 8) << '.'
			<< (addr_int & 0xff) << ':'
			<< port_int << '\n';
	}
	
	{
	getsockname(sender_fd, (sockaddr*)&sender_addr, &sender_addrlen);
	uint32_t	addr_int = ntohl(sender_addr.sin_addr.s_addr);
	uint32_t	port_int = ntohs(sender_addr.sin_port);
	std::cout << "accpeted socket information ";
	std::cout << (addr_int >> 24) << '.'
			<< ((addr_int & 0xff0000) >> 16) << '.'
			<< ((addr_int & 0xff00) >> 8) << '.'
			<< (addr_int & 0xff) << ':'
			<< port_int << '\n';
	}

	char buffer[16] = {0, };
	while (read(sender_fd, buffer, 15) >= 0)
	{
		std::cout << "received >> ";
		std::cout << buffer << '\n';
	}
	std::cout << "recipient end\n";
}
