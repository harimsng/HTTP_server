#include <sys/_types/_ssize_t.h>
#include <unistd.h>
#include <iostream>

static ssize_t	write_wrapper(int fd, const void* buf, size_t size);

int	main(int argc, char** argv, char** envp)
{
	int	fdset[2];
	int	pid;
	char	buf[1000000];

	static_cast<void>(argc);
	static_cast<void>(argv);
	pipe(fdset);
	pid = fork();
	if (pid == 0)
	{
		close(fdset[1]);
		dup2(fdset[0], 0);
		execve("./cgi_tester", NULL, envp);
	}
	close(fdset[0]);
	memset(buf, 'a', 32768);
	write_wrapper(fdset[1], buf, 32768);
	wait(NULL);
}

static ssize_t	write_wrapper(int fd, const void* buf, size_t size)
{
	size_t		totalWriteBytes = 0;
	uintptr_t	ptr = reinterpret_cast<uintptr_t>(buf);

	while (size > 0)
	{
		void*	movedBuf = reinterpret_cast<void*>(ptr + totalWriteBytes);
		ssize_t	writeBytes = write(fd, movedBuf, size);

		if (writeBytes == -1)
			continue;
		totalWriteBytes += writeBytes;
		size -= writeBytes;
	}
	return totalWriteBytes;
}
