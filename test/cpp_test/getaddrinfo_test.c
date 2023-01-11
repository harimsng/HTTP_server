#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <err.h>

#define MAXSOCK (64)

void	getaddrinfo_wrapper(const char *hostname, const char *service);

int	main()
{
	getaddrinfo_wrapper(NULL, "http");
}

void	getaddrinfo_wrapper(const char *hostname, const char *service)
{
       struct addrinfo hints, *res, *res0;
       int error;
//       int s[MAXSOCK];
       int nsock;
//       const char *cause = NULL;

       memset(&hints, 0, sizeof(hints));
	   /*
       hints.ai_family = PF_UNSPEC;
       hints.ai_socktype = SOCK_STREAM;
       hints.ai_flags = AI_PASSIVE;
	   */
       error = getaddrinfo(hostname, service, &hints, &res0);
       if (error) {
               errx(1, "%s", gai_strerror(error));
               /*NOTREACHED*/
       }
       nsock = 0;
       for (res = res0; res && nsock < MAXSOCK; res = res->ai_next) {

             printf("ai_flags = %d\nai_family = %d\nai_socktype = %d\nai_protocol = %d\n\
ai_addrlen = %u\nai_addr = %x\nai_port = %u\nai_canonname = %s\n\n", res->ai_flags, res->ai_family,
             res->ai_socktype,        /* socket type */
             res->ai_protocol,        /* protocol for socket */
             res->ai_addrlen,   /* length of socket-address */
             ntohl(((struct sockaddr_in *)res->ai_addr)->sin_addr.s_addr), /* socket-address for socket */
             ntohs(((struct sockaddr_in *)res->ai_addr)->sin_port), /* socket-address for socket */
             res->ai_canonname);     /* canonical name for service location */
     	}
       freeaddrinfo(res0);
}
