#include <unistd.h>

int	childProcess(int pipeFd[2]);

int	main()
{
	int	pid;
	int	pipeFd[2];

	pipe(pipeFd);
	pid = fork();
	if (pid == 0)
		return childProcess(pipeFd);
	close(pipeFd[1]);
	sleep(1);

	char	buf[1024] = {0, };
	int		count;

	while ((count = read(pipeFd[0], buf, 10)) > 0)
	{
		write(STDOUT_FILENO, "read=\"", 6);
		write(STDOUT_FILENO, buf, count);
		write(STDOUT_FILENO, "\"\n", 2);
	}
	while ((count = read(pipeFd[0], buf, 10)) == 0)
	{
		sleep(1);
		write(STDOUT_FILENO, "0\n", 2);
	}
}

int	childProcess(int pipeFd[2])
{
	close(pipeFd[0]);
	write(pipeFd[1], "12345", 5);
	return 123;
}
