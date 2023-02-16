#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <cstdlib>

#define BUFFER_SIZE (64)
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
//	unsigned char	optionValue[4];
//	socklen_t		optionLen = sizeof(optionValue);

	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	buffer[BUFFER_SIZE] = 0;
	printf("parent work starts\n");
	while (1)
	{
		sleep(2);
		count = read(randomFd, buffer, BUFFER_SIZE);
		if (count == -1)
		{
			fprintf(stderr, "read fail: %s\n", strerror(errno));
			continue;
		}
		for (int i = 0; i < count - 1; ++i)
		{
			buffer[i] = (static_cast<unsigned int>(buffer[i]) % 63) + 64;
			buffer[i] = 0;
		}
		fprintf(stderr, "randomized bytes\n{\n%s\n}\n\n", buffer);
		buffer[count - 1] = '\0';
//		if (nwrite < 1)
		{
//			continue;
		}
		count = write(clientSocket, buffer, count);
		if (count <= 0)
		{
			fprintf(stderr, "write to clientSocket fail: %s\n", strerror(errno));
			continue;
		}
		/*
		optionLen = sizeof(optionValue);
		getsockopt(clientSocket, SOL_SOCKET, SO_NREAD, &optionValue, &optionLen);
		unsigned int nread = *(unsigned int *)optionValue;
		fprintf(stderr, "nread = %u, bufferlen = %u\n", nread, optionLen);
		if (nread < 1)
		{
			continue;
		}
		*/
		count = read(clientSocket, buffer, BUFFER_SIZE);
		if (count <= 0)
		{
			fprintf(stderr, "read from clientSocket fail: %s\n", strerror(errno));
			continue;
		}
		printf("converted %d bytes\n{\n%s\n}\n\n", count, buffer);
	}
	wait(NULL);
}

int	child()
{
	/*
	int		count = 0;
	char	buffer[BUFFER_SIZE + 1] = {0, };
	*/
	int		socketFd = socket(AF_LOCAL, SOCK_STREAM, 0);
	struct sockaddr_un	addr;

	addr.sun_family = AF_LOCAL;
	::strlcpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
	addr.sun_len = SUN_LEN(&addr);

	sleep(2);
	if (connect(socketFd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
	{
		fprintf(stderr, "connect fail: \"%s\"\n", strerror(errno));
	}
	printf("child work starts\n");
	dup2(socketFd, STDIN_FILENO);
	dup2(socketFd, STDOUT_FILENO);
	char*	argv[3];

	argv[0] = (char*)"cat";
//	argv[1] = 0;
	argv[1] = (char*)"-e";
	argv[2] = 0;
	return execvp("cat", argv);
//	return execlp("gsed", "gsed", "-e", "s/[a-z]/\\U\\0/g", NULL);
	/*
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
	*/
}
