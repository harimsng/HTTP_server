#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>

#include <iostream>

int	main()
{
	int		socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
	int		pid;

	pid = fork();
	if (pid < 0)
	{
		std::cout << "fork error\n";
		return 1;
	}
	if (pid == 0)
	{
		std::string	message = "message from child process"; 
		ssize_t		count;

		count = write(socket_fd, message.data(), message.size());
		if (count < 0)
			std::cout << "child process errno = " << errno << ", error message = " << strerror(errno) << '\n';
		close(socket_fd);
	}
	else
	{
		char	buffer[128] = {0, };
		ssize_t	count;

		count = read(socket_fd, buffer, 128);
		std::cout << "count = " << count << '\n';
		std::cout << "message = \"" << buffer << "\"\n";
		if (count < 0)
			std::cout << "parent process errno = " << errno << ", error message = " << strerror(errno) << '\n';
	}
}
