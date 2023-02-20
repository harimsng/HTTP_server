#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <iostream>

int main()
{
	int		infile_fd = open("Makefile", O_RDONLY);
	int		outfile_fd = open("poll_test_file.txt", O_WRONLY | O_CREAT);
	struct pollfd	event_list[2];
	struct pollfd	read_event;
	struct pollfd	write_event;
	
	printf("POLLIN = %d, POLLOUT = %d\n", POLLIN, POLLOUT);
	read_event.fd = infile_fd;
	read_event.events = POLLIN;
	write_event.fd = outfile_fd;
	write_event.events = POLLOUT;
	printf("infile fd = %d, outfile fd = %d\n", read_event.fd, write_event.fd);
	event_list[0] = read_event;
	event_list[1] = write_event;
	poll(event_list, static_cast<nfds_t>(2), 1);
	printf("event_list\n");
	printf("fd = %d, revent = %d\n", event_list[0].fd, event_list[0].revents);
	printf("fd = %d, revent = %d\n", event_list[1].fd, event_list[1].revents);
}
