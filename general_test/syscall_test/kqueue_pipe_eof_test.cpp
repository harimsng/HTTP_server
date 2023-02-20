#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

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

	int	kfd = kqueue();

	struct kevent	kEvent;
	struct kevent	kResult;

	EV_SET(&kEvent, pipeFd[0], EV_ADD, EVFILT_READ, 0, 0, 0);

	char	buf[1024] = {0, };
	int		count;

	struct timespec	timeout;

	while (1)
	{
		timeout = (struct timespec){1000000, 1000};
		kevent(kfd, &kEvent, 1, &kResult, 1, &timeout);
		count = read(pipeFd[0], buf, 10);
		write(STDOUT_FILENO, "read=\"", 6);
		write(STDOUT_FILENO, buf, count);
		write(STDOUT_FILENO, "\"\n", 2);
	}
	/*
	while ((count = read(pipeFd[0], buf, 10)) == 0)
	{
		sleep(1);
		write(STDOUT_FILENO, ".\n", 2);
	}
	*/
}

int	childProcess(int pipeFd[2])
{
	close(pipeFd[0]);
	write(pipeFd[1], "12345", 5);
	return 123;
}
