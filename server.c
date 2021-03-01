/*
 * @author: Álvaro Merino Argumánez
 * NIA : 100406665
 * contact : 100406665@alumnos.uc3m.es
 * 
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <time.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024
#define TIMESTAMP_DELTA 2208988800ul

int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 
int server(int port, int debug) 
{ 
    int listenfd, connfd, udpfd, nready, maxfdp1; 
    char buffer[BUFSIZE]; 
    
    pid_t childpid; 
    fd_set rset; 
    ssize_t n; 
    socklen_t len; 
    const int on = 1; 
    struct sockaddr_in cliaddr, servaddr; 

    //char* testMessage = "Hello Client";

    timer_t rawtime;
    uint32_t numericalmsg;
    void sig_chld(int); 

    /* 
     * SOCK_DGRAM VS SOCK_STREAM
     * 
     * SOCK_DGRAM : datagram-based protocol.
     * Envías un datagrama, recibes una respuesta y la conexión termina ahí.
     * 
     * SOCK_STREAM : connection-based protocol.
     * La conexión se establece y cliente-servidor intercambian información
     * hasta que uno de los dos cierra la conexión o se produce un error de red.
     */
  
    // Create listening TCP socket

    listenfd = socket(AF_INET, SOCK_STREAM, 0); 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port); 
  
    // Binding server addr structure to listenfd 

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    listen(listenfd, 10); 
  
    /* Create UDP socket */

    udpfd = socket(AF_INET, SOCK_DGRAM, 0); 

    // binding server addr structure to udp sockfd 

    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
  
    // Clear the descriptor set 
    FD_ZERO(&rset); 

    if(debug) printf("listenfd value is : %d\n", listenfd);
    if(debug) printf("udpfd value is : %d\n", udpfd);
  
    // Get maxfd (sumamos uno porque select recorre de 0 a maxfdp1 - 1)
    maxfdp1 = max(listenfd, udpfd) + 1;

    for (;;) { 

        // Set listenfd and udpfd in readset 
        FD_SET(listenfd, &rset); 
        FD_SET(udpfd, &rset); 
  
        // Select the ready descriptor 
        nready = select(maxfdp1, &rset, NULL, NULL, NULL); 

        /* 
         * int select(int maxfd, fd_set *readset, fd_set *writeset, 
         *          fd_set *exceptset, const struct timeval *timeout);
         * 
         * maxfd: maximum number of descriptor ready.
         * readset: Descriptor set that we want kernel to test for reading.
         * writeset: Descriptor set that we want kernel to test for writing.
         * exceptset: Descriptor set that we want kernel to test for exception conditions.
         * timeout: How long to wait for select to return.
         * 
         * FD_ISSET(fd, &fdset)  
         * Returns a non-zero value if the bit for the file descriptor fd 
         * is set in the file descriptor set pointed to by fdset, and 0 otherwise. 
         * 
         * Devuelve distinto de cero si el bit del fd introducido está fijado 
         * en el fd apuntado al fdset, y cero en caso contrario.
         */

        /*******************************************************************/
  
        /* If TCP socket is readable then handle 
         * it by accepting the connection 
         */

        if (FD_ISSET(listenfd, &rset)) { 

            len = sizeof(cliaddr); 
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len); 

            if ((childpid = fork()) == 0) { 
                
                bzero(buffer, sizeof(buffer)); 
                close(listenfd);
                TCPtimed(connfd, debug, servaddr);
                close(connfd);
                exit(0);
            }
        } 

        // If UDP socket is readable receive the message. 

        if (FD_ISSET(udpfd, &rset)) { 

            len = sizeof(cliaddr); 
            bzero(buffer, sizeof(buffer)); 
            printf("Packet received from UDP client (%s)\n", inet_ntoa(cliaddr.sin_addr)); 
            n = recvfrom(udpfd, buffer, sizeof(buffer), 0, 
                         (struct sockaddr*)&cliaddr, &len); 
            puts(buffer); 
            
            rawtime = time(NULL);
            //info = localtime(&rawtime);
            numericalmsg = htonl(((uint32_t) rawtime) + TIMESTAMP_DELTA);

            sendto(udpfd, (uint32_t*)&numericalmsg, sizeof(buffer), 0, 
                   (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 

            printf("Local time is : %s\n", ctime((const time_t*)&rawtime));
        } 
    } 
}

/*------------------------------------------------------------------------
 * sigchld_handler - reaper of zombie child processes
 *------------------------------------------------------------------------
 */
void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

/*------------------------------------------------------------------------
 * TCPtimed - send current time every second until end of file
 *------------------------------------------------------------------------
 */
int TCPtimed(int fd, int debug, struct sockaddr_in servaddr) {

    char buf[BUFSIZE];
    timer_t serverTime;
    uint32_t numericalTime;

    while(1) {

        //read(fd, buf, sizeof(buf)); 
        //printf("Packet received from TCP client (%s)\n", inet_ntoa(cliaddr.sin_addr));  
        //puts(buf);

        sleep(1);
                
        serverTime = time(NULL);
        //info = localtime(&rawtime);
        numericalTime = htonl(((uint32_t) serverTime) + TIMESTAMP_DELTA);

        //memcpy(buf, (char*)&numericalTime, sizeof(uint32_t));

        sendto(fd, (int *)&numericalTime, sizeof(uint32_t), MSG_CONFIRM, 
          (const struct sockaddr *)&servaddr, sizeof(servaddr));

        //write(fd, numericalTime, sizeof(uint32_t)); 
        //if(debug) printf("Local time is : %s\n", ctime((const time_t*)&serverTime));  
          
    }

    return 0;
}

#pragma GCC diagnostic pop