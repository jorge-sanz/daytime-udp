// Practica tema 5, Sanz Pérez Jorge

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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
    char buffer[32];
    int n;
    struct servent *application_name;
    unsigned int server_length;

    /* check if there is at least one program argument */
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s address.IP.server port-number\n", argv[0]);
        exit(0);
    }

    /* get port number from arguments */
    if (argv[2])
    {
        port = htons(atoi(argv[2])); /* Transform the port to network byte order */
        printf("port assigned by program argument");
    }
    else
    { /* get the port by getservbyname when is not passed as argument */
        application_name = getservbyname("daytime", "udp");

        if (!application_name)
        {
            printf("unknown application daytime\n");
            exit(0);
        }
        else
        {
            port = application_name->s_port;
            printf("getservbyname was successful\n");
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

    strcpy(buffer, "Hello");

    /* send the message in buffer to the server */
    server_length = sizeof(server);
    printf("before sending... port 0x%x address 0x%x \n", server.sin_port, server.sin_addr.s_addr);
    n = sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *) &server, server_length);
    printf("after sending...\n");
    if (n < 0)
        error("ERROR in sendto");

    /* receive the message from server */
    printf("before receiving...\n");
    fflush(stdout);
    server_length = sizeof(server);
    n = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *) &server, &server_length);
    printf("after receiving...\n");
    fflush(stdout);
    if (n < 0)
        error("ERROR in recvfrom");

    /* print server reponse */
    printf("Echo from server: %s", buffer);
    close(s);
    return 0;
}
