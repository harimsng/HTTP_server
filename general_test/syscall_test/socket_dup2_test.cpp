#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cerrno>
#include <cstring>

#define BUFFER_SIZE (128)
#define SOCKET_PATH "/tmp/temp_socket"

int	child();

int	main()
{
	int		listenSocket;
	int		pid;

	unlink(SOCKET_PATH);
	pid = fork();
	if (pid < 0)
	{
		printf("fork failed\n");
	}
	else if (pid == 0)
	{
		return child();
	}

	// listen socket binding
	{
		struct sockaddr_un	addr;

		addr.sun_family = AF_LOCAL;
		::strlcpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
		addr.sun_len = SUN_LEN(&addr);
		listenSocket = socket(AF_LOCAL, SOCK_STREAM, 0);

		if (bind(listenSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0
		|| listen(listenSocket, 2) < 0)
		{
			fprintf(stderr, "bind error: %s\n", strerror(errno));
		}
	}

	struct sockaddr_un	clientAddr;
	socklen_t			clientAddrLen = sizeof(clientAddr);
	fprintf(stderr, "clientAddrLen = %u before\n", clientAddrLen);
	int		clientSocket = accept(listenSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
	fprintf(stderr, "clientAddrLen = %u after\n", clientAddrLen);

	char	buffer[BUFFER_SIZE + 1] = {0, };
	int		count;
	int		randomFd = open("/dev/random", O_RDONLY);

	buffer[BUFFER_SIZE] = 0;
	printf("parent work starts\n");
	while (1)
	{
		sleep(2);
		count = read(randomFd, buffer, BUFFER_SIZE);
		if (count == -1)
			continue;
		for (int i = 0; i < count; ++i)
		{
			buffer[i] = (static_cast<unsigned int>(buffer[i]) % 64) + 64;
		}
		printf("randomized bytes\n{\n%s\n}\n\n", buffer);
		write(clientSocket, buffer, BUFFER_SIZE);
		count = read(clientSocket, buffer, BUFFER_SIZE);
		printf("converted %d bytes\n{\n%s\n}\n\n", count, buffer);
	}
}

int	child()
{
	int		count = 0;
	char	buffer[BUFFER_SIZE + 1] = {0, };
	int		socketFd = socket(AF_LOCAL, SOCK_STREAM, 0);
	struct sockaddr_un	addr;

	addr.sun_family = AF_LOCAL;
	::strlcpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
	addr.sun_len = SUN_LEN(&addr);

	sleep(1);
	if (connect(socketFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
	{
		fprintf(stderr, "connect fail: \"%s\"\n", strerror(errno));
	}
	printf("child work starts\n");
	dup2(socketFd, STDIN_FILENO);
	dup2(socketFd, STDOUT_FILENO);
	while (1)
	{
		sleep(2);
		if (count != 0)
		{
			int	written = write(STDOUT_FILENO, buffer, count);
			if (written != -1)
				count -= written;
			continue;
		}
		count = read(STDIN_FILENO, buffer, BUFFER_SIZE);
		if (count == -1)
		{
			fprintf(stderr, "read fail: \"%s\"\n", strerror(errno));
		}
		for (int i = 0; i < count; ++i)
		{
			if (buffer[i] >= 'a' && buffer[i] <= 'z')
				buffer[i] &= ~32;
		}
		if (count <= 0)
		{
			count = 0;
			continue;
		}
		int	written = write(socketFd, buffer, count);
		if (written != -1)
			count -= written;
	}
}
