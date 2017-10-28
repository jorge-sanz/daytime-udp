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
void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
  }

int main(int argc, char **argv)
{
    int s; /* socket */
    int port; /* port number */
    struct sockaddr_in server; /* server structure */
    char buffer[BUFSIZE]; /* messages buffer */
    int n; /* flag variable for errors */
    struct servent *application_name; /* application structure */
    unsigned int server_length; /* length of server structure */

    /* check if there is at least one program argument */
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s address.IP.server (mandatory) port-number (optional)\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* get port number from arguments */
    if (argv[2])
    {
        port = htons(atoi(argv[2]));
    }
    else
    { /* get the port by getservbyname when is not passed as argument */
        application_name = getservbyname("daytime", "udp");

        if (!application_name)
        {
            printf("unknown application daytime\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            port = application_name->s_port;
        }
    }

    /*
    * create a datagram socket in the internet domain and use the
    * default protocol (UDP)
    */
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
        error("ERROR opening socket");

    /* set up the server name */
    server.sin_family = AF_INET;                 /* Internet Domain    */
    server.sin_port = port;                      /* Server Port        */
    server.sin_addr.s_addr = inet_addr(argv[1]); /* Server's Address   */

    /* set the message to be send */
    strcpy(buffer, "Hello");

    /* send the message in buffer to the server */
    server_length = sizeof(server);
    n = sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *) &server, server_length);
    if (n < 0)
        error("ERROR in sendto");

    /* receive the message from server */
    fflush(stdout);
    server_length = sizeof(server);
    n = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *) &server, &server_length);
    fflush(stdout);
    if (n < 0)
        error("ERROR in recvfrom");

    /* print server response */
    printf("%s", buffer);
    close(s);
    return 0;
}
