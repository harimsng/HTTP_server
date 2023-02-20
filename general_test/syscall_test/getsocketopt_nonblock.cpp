#include <cstring>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

#include <fcntl.h>
#include <unistd.h>

#include <exception>
#include <iostream>

static void	sender();
static void	receiver();

int	main()
{
	int	pid;

	pid = fork();
	if (pid == 0)
	{
		sender();
		return 0;
	}
	receiver();
	return 0;
}

void	sender() try
{
	int	sndSock = socket(PF_INET, SOCK_STREAM, 0);
	sockaddr_in	sockAddr;
	unsigned char	buffer[128] = {0, };

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_len = INET_ADDRSTRLEN;
	sockAddr.sin_addr = (in_addr){htonl(INADDR_LOOPBACK)};
	sockAddr.sin_port = htons(6667);
	memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));

	sleep(1);
	std::cout << "sender try connection\n"; 
	if (connect(sndSock, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) == -1)
		throw std::runtime_error(std::string(std::strerror(errno)) + ": connection fail");

	std::cout << "sender connection success\n"; 
	while (1)
	{
		memset(buffer, rand() % 96 + 32, 16);
		int	writeLen = write(sndSock, buffer, 16);

		std::cout << "writeLen = " << writeLen << "\n";
		sleep(2);
	}
	close(sndSock);
}
catch (std::exception& e)
{
	std::cerr << e.what() << '\n';
}

void	receiver() try
{
	fd_set	readSet;
	int		listenSock = socket(PF_INET, SOCK_STREAM, 0);
	int		rcvSock;
	sockaddr_in	sockAddr;
	socklen_t	sockLen = 0;
	unsigned char	buffer[1024] = {0, };
	ssize_t			readLen = 0;
	unsigned char	sockoptBuffer[128] = {0, };
	socklen_t		sockoptLen = 0;

	fcntl(listenSock, F_SETFL, O_NONBLOCK);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_len = INET_ADDRSTRLEN;
	sockAddr.sin_addr = (in_addr){htonl(INADDR_ANY)}; 
	sockAddr.sin_port = htons(6667);
	memset(sockAddr.sin_zero, 0, sizeof(sockAddr.sin_zero));
	if (bind(listenSock, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) == -1)
		throw std::runtime_error("bind fail: " + std::string(std::strerror(errno)));
	if (listen(listenSock, 8) == -1)
		throw std::runtime_error("listen fail: " + std::string(std::strerror(errno)));
	std::cout << "receiver ready\n";
	FD_ZERO(&readSet);
	FD_SET(listenSock, &readSet);
	sockLen = sizeof(sockAddr);
	if (select(64, &readSet, &readSet, NULL, NULL) == -1)
		throw std::runtime_error("select fail: " + std::string(std::strerror(errno)));
	rcvSock = accept(listenSock, reinterpret_cast<sockaddr*>(&sockAddr), &sockLen);
	if (rcvSock == -1)
		throw std::runtime_error("accept fail: " + std::string(std::strerror(errno)));
	std::cout << "connection established\n";
	while (1)
	{
		sleep(2);
		bzero(buffer, 1024);
		readLen = read(rcvSock, buffer, 128);
		buffer[readLen] = '\0';
		std::cout << "readLen = " << readLen << "\n";
		std::cout << "content = " << buffer << "\n";
		getsockopt(rcvSock, 6, SO_ERROR, sockoptBuffer, &sockoptLen);
		if (sockoptLen > 127)
			throw std::runtime_error("sockoptlen too long");
		if (sockoptLen <= 0)
			continue;
		sockoptBuffer[sockoptLen] = '\0';
		std::cout << "getsockopt = " << sockoptBuffer << "\n";
	}
}
catch (std::exception& e)
{
	std::cerr << e.what() << '\n';
}
