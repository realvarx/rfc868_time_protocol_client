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

#include "tcp_client.h"
#include "udp_client.h"

#define BUFSIZE 1024
#define TIMESTAMP_DELTA 2208988800ul

int sockfd;

void sigint_handler_tcp(int signo)
{
    //things to do after a Ctrl-C, before finishing the program
    puts("SIGINT received, closing program...");
    exit(0);
    
}


int tcp_client(char *host, int port, int debug)
{
        struct hostent *server;
	struct sockaddr_in serveraddr;

    char buf[BUFSIZE+1];
    int n;
    int outchars, inchars;
    uint32_t packet;

        /* handler SIGINT signal*/
	signal(SIGINT, sigint_handler_tcp);

        //while(1){

        /* socket: create the socket */
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
                perror("ERROR opening socket");
                exit(0);
        }

        /* gethostbyname: get the server's DNS entry */
        server = gethostbyname(host);
        if (server == NULL) {
                fprintf(stderr,"ERROR, no such host: %s\n", host);
                exit(0);
        }

        /* build the server's Internet address */
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(port);

        /* connect: create a connection with the server */
        if (connect(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
                perror("ERROR connecting");
                exit(0);
        }

        int recv_value;

        /* get message line from the user */
        while (1) {

                
                /* send the message line to the server */
                //buf[BUFSIZE] = '\0';    /* insure line null-terminated  */
                /*memset(buf, 0, sizeof(buf));
                outchars = sizeof(uint32_t);
                n = write(sockfd, buf, sizeof(buf));
                if (n < 0) {
                        perror("ERROR writing to socket");
                        exit(0);
                }*/

		/* read answer from the server */
                //for (inchars = 0; inchars < outchars; inchars+=n ) {
                //bzero(buf, sizeof(buf));

        uint32_t recv_data = 0;

                /*n = read(sockfd, (uint32_t)&recv, sizeof(uint32_t));
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(0);
                }*/
                //}
                /* print the server's reply */

        socklen_t *len = NULL;

        /*recv_value = recvfrom(sockfd, (uint32_t *)&recv, sizeof(uint32_t), MSG_WAITALL, 
        (struct sockaddr *)&serveraddr, len);*/
        //recv_value = recv(sockfd, &recv, sizeof(uint32_t), 0);
        recv_value = recv(sockfd, (uint32_t *)&recv_data, sizeof(uint32_t), 0);
                //memcpy(&packet, buf, 32);
        if(debug) printf("%d\n", recv_value);

        if(recv_value == 0) {
                close(sockfd);
                exit(0);
        }
                //packet = ntohl(packet);
        recv_data = ntohl(recv_data);
                //if(debug) puts("Antes del casting y despues del ntohl");

        time_t time = (time_t) (recv_data-TIMESTAMP_DELTA);

                //puts("Antes del printf");

                //buffer[n] = "\0";
        printf("Server time is : %s\n", ctime((const time_t*)&time));

        if(sockfd == 0) {
           close(sockfd);
           exit(0);
        }
   
        }
        //close(sockfd);
        //recv = 0;
        //}
	return(0);
}
