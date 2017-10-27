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
    exit(1);
  }

int main(int argc, char **argv)
{
    int s;
    int port;
    struct sockaddr_in server;
    char message[32];
    int n;
    struct servent *appl_name;
    unsigned int serverlen;

    /* check if there is at least one program argument */
    if (argc < 2)
    {
        fprintf(stderr, "usage: %s address.IP.server [port]\n", argv[0]);
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

    strcpy(message, "Hello");

    /* send the message in message to the server */
    serverlen = sizeof(server);
    printf("before sending... port 0x%x address 0x%x \n", server.sin_port, server.sin_addr.s_addr);
    n = sendto(s, message, strlen(message), 0, (struct sockaddr *) &server, serverlen);
    printf("after sending...\n");
    if (n < 0)
        error("ERROR in sendto");

    /* receive the message from server */
    printf("before receiving...\n");
    fflush(stdout);
    serverlen = sizeof(server);
    n = recvfrom(s, message, sizeof(message), 0, (struct sockaddr *) &server, &serverlen);
    printf("after receiving...\n");
    fflush(stdout);
    if (n < 0)
        error("ERROR in recvfrom");

    /* print server reponse */
    printf("Echo from server: %s", message);
    close(s);
    return 0;
}
