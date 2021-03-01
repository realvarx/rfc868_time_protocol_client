/*
 * @author: Álvaro Merino Argumánez
 * NIA : 100406665
 * contact : 100406665@alumnos.uc3m.es
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

#include "udp_client.h"

#define PORT 6665
//#define BUFSIZE 1024
#define TIMESTAMP_DELTA 2208988800ul

void sigint_handler_udp(int signo)
{
	//things to do after a Ctrl-C, before finishing the program
	puts("SIGINT received, closing program");
	exit(0);
}

int udp_client(char *host, int port, int debug) {

    int sockfd; // Socket file descriptor
    //char buffer[BUFSIZE];
    uint32_t packet;
    struct hostent *server;
    struct sockaddr_in servaddr;

    /* handler SIGINT signal*/
	signal(SIGINT, sigint_handler_udp);

    // Creating the file descriptor

    if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {    
        perror("udp_client : socket couldn't be created");
        exit(EXIT_FAILURE);
    }

    /* gethostbyname: get the server's DNS entry */
    if(debug) printf("El host que está entrando es : %s\n", host);
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host: %s\n", host);
        exit(0);
    }

    if(debug) puts("Antes de los memory set cpy");

    /* build the server's Internet address and fill server's information */
    memset((char*)&servaddr, 0, sizeof(servaddr));
    memcpy((char*)&servaddr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    /* sendto(int __fd, const void *__buf, size_t __n, int __flags, 
            const struct sockaddr *__addr, socklen_t __addr_len) */

    //int n;
    socklen_t *len = NULL;

    //if(connect())
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    { 
        printf("\n Error : Connect Failed \n"); 
        exit(0); 
    }

    //Send N bytes of BUF on socket FD to peer at address ADDR (which is
    //    ADDR_LEN bytes long). Returns the number sent, or -1 for errors. 

    if(debug) puts("Antes del sendto");

    sendto(sockfd, (uint32_t *)&packet, sizeof(uint32_t), MSG_CONFIRM, 
          (const struct sockaddr *)&servaddr, sizeof(servaddr));
    // recvfrom(int __fd, void *__restrict__ __buf, size_t __n, int __flags, 
    //          struct sockaddr *__restrict__ __addr, socklen_t *__restrict__ __addr_len) 

    if(debug) puts("Ha pasado el sendto");

    recvfrom(sockfd, (uint32_t *)&packet, sizeof(uint32_t), MSG_WAITALL, 
                (struct sockaddr *)&servaddr, len);

    //if(n == -1) return -1;

    if(debug) puts("Ha pasado el recvfrom");

    // The number of seconds correspond to the seconds passed since 1900.
    // ntohl() converts the bit/byte order from the network's to host's "endianness".

    packet = ntohl(packet);

    if(debug) puts("Antes del casting y despues del ntohl");

    time_t time = (time_t) (packet-TIMESTAMP_DELTA);

    //puts("Antes del printf");

    //buffer[n] = "\0";
    printf("Server time is : %s\n", ctime((const time_t*)&time));

    close(sockfd);

    return 0;
}
