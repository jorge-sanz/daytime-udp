// Practica tema 5, Sanz Pérez Jorge

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUFSIZE 1024

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
	int client_length;
	struct servent *appl_name;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	char buffer[BUFSIZE];
	int n; /* message byte size */
	char hostname[128]; /* server hostname */
	FILE *file;
	char response[BUFSIZE];	

	/* check arguments */
	if (argc < 1)
	{
		fprintf(stderr, "usage: %s port (optional)\n", argv[0]);
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
			printf("set port by servbyname()\n");
		}
	}

	printf("port: %d\n", port);

	/* socket: create the parent socket */
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		error("ERROR opening socket");

	server_address.sin_family = AF_INET; /* server is in Internet Domain */
	server_address.sin_port = port; /* use any available port */
	server_address.sin_addr.s_addr = INADDR_ANY; /* server's Internet Address */

	/* bind: associate the parent socket with a port */
	if (bind(s, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
		error("ERROR on binding");

	/* main loop: wait for a datagram and reply with server name and time */
	client_length = sizeof(client_address);
	while (1)
	{
		/* recvfrom: receive a UDP datagram from a client */
		bzero(buffer, BUFSIZE);
		n = recvfrom(s, buffer, BUFSIZE, 0, (struct sockaddr *) &client_address, (socklen_t *) &client_length);
		if (n < 0)
			error("ERROR in recvfrom");

		/* get server hostname */
		gethostname(hostname, sizeof hostname);
		printf("My hostname: %s\n", hostname);

		/* print received message */
		printf("server received datagram: %s\n", buffer);

		/* get and print current daytime */
		system("date > /tmp/tt.txt");
		bzero(buffer, BUFSIZE);
		file = fopen("/tmp/tt.txt", "r");
		if (fgets(buffer, BUFSIZE, file) == NULL)
		{
			printf("Error in system(), in fopen(), or in fgets()\n");
			exit(EXIT_FAILURE);
		}
		printf("Date: %s", buffer);

		/* concat server name + date */
		strcpy(response, hostname);
		strcat(response, ": ");
		strcat(response, buffer);
		printf("%s\n", response);

		/* send server name and date to client */
		n = sendto(s, response, strlen(response), 0, (struct sockaddr *) &client_address, client_length);
		if (n < 0)
			error("ERROR in sendto");
	}

	return 0;
}
