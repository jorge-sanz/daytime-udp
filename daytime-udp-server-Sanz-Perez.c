// Practica tema 5, Sanz Pérez Joge

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUFSIZE 32

/* wrapper for perror */
void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char **argv)
{
	int port; /* port to listen on */
	int s;	/* socket */
	int clientlen;
	struct servent *appl_name;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	char buffer[32];
	int n;				/* message byte size */
	char hostname[128]; /* server hostname */
	FILE *file;

	/* check arguments */
	if (argc < 1)
	{
		fprintf(stderr, "usage: %s port\n", argv[0]);
		exit(0);
	}

	/* get port number from arguments */
	if (argv[1])
	{
		port = htons(atoi(argv[1]));
		printf("port assigned by program argument\n");
	}
	else /* get port number by getservbyname */
	{
		appl_name = getservbyname("daytime", "udp");

		if (!appl_name)
		{
			printf("unknown application daytime\n");
			exit(0);
		}
		else
		{
			port = appl_name->s_port;
			printf("getservbyname was successful\n");
		}
	}

	printf("port: %d\n", port);

	/* socket: create the parent socket */
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		error("ERROR opening socket");

	serveraddr.sin_family = AF_INET;		 /* Server is in Internet Domain */
	serveraddr.sin_port = port;				 /* Use any available port */
	serveraddr.sin_addr.s_addr = INADDR_ANY; /* Server's Internet Address */

	/* bind: associate the parent socket with a port */
	if (bind(s, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		error("ERROR on binding");

	/* main loop: wait for a datagram, then echo it */
	clientlen = sizeof(clientaddr);
	while (1)
	{
		/* recvfrom: receive a UDP datagram from a client */
		bzero(buffer, 32);
		n = recvfrom(s, buffer, BUFSIZE, 0, (struct sockaddr *)&clientaddr, (socklen_t *)&clientlen);
		if (n < 0)
			error("ERROR in recvfrom");

		/* get server hostname */
		gethostname(hostname, sizeof hostname);
		printf("My hostname: %s\n", hostname);

		/* print received message */
		printf("server received datagram: %s\n", buffer);

		/* get and print current daytime */
		system("date > /tmp/tt.txt");
		bzero(buffer, 32);
		file = fopen("/tmp/tt.txt", "r");
		if (fgets(buffer, BUFSIZE, file) == NULL)
		{
			printf("Error in system(), in fopen(), or in fgets()\n");
			exit(EXIT_FAILURE);
		}
		printf("Date: %s\n", buffer);

		/* send date to client */
		n = sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *) &clientaddr, clientlen);
		if (n < 0)
			error("ERROR in sendto");
	}

	return 0;
}
