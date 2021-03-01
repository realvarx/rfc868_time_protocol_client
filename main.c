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
#include <time.h>

#include "udp_client.h"
#include "tcp_client.h"
#include "server.h"



int main(int argc, char *argv[])
{
    /*
    print("          _      _       _       \n     /\  | |    | |     | |      \n"
    "    /  \ | |_ __| | __ _| |_ ___ \n   / /\ \| __/ _` |/ _` | __/ _ \\\n"
    "  / ____ \ || (_| | (_| | ||  __/\n /_/    \_\__\__,_|\__,_|\__\___|\n"
    "                                 \n                                 ");
    print("Álvaro Merino Argumánez - APT 2020\n\n");
    */

    char* host;
    char* execution_mode = "cu";
	int port = 37;
    int debug = 0;

    if(argc < 2) {
        fprintf(stderr, "usage: %s [-s <serverhost>] [-p <port>] [-m <cu\\ct\\s>] [-d]\n", argv[0]);
		exit(1);
    }

    for(int i = 0; i<argc; i++) {
        if(strcmp(argv[i], "-s") == 0) host = argv[i+1];
        if(strcmp(argv[i], "-p") == 0) port = atoi(argv[i+1]);
        if(strcmp(argv[i], "-m") == 0) execution_mode = argv[i+1];
        if(strcmp(argv[i], "-d") == 0) debug = atoi(argv[i+1]);
    }

    if(((strcmp(execution_mode, "cu") == 0) || (strcmp(execution_mode, "ct") == 0)) && host == NULL) {
        fprintf(stderr, "[-s serverhost] es un parámetro requerido al arrancar en modo consulta\n");
		exit(1);
    }

    if(port != 37) {
        if((strcmp(execution_mode, "cu") == 0) || (strcmp(execution_mode, "ct") == 0)) 
        if(debug) printf("El servidor está escuchando en %d\n", port);
        if(strcmp(execution_mode, "s")) printf("TIME server running in port %d\n", port);
    }

    

    if(strcmp(execution_mode, "cu") == 0) {
        if(debug) puts("UDP CLIENT MODE\n");
        udp_client(host, port, debug);
    }

    if(strcmp(execution_mode, "ct") == 0) {
        if(debug) puts("TCP CLIENT MODE\n");
        tcp_client(host, port, debug);
    }
    
    if(strcmp(execution_mode, "s") == 0) {
        if(debug) puts("CONCURRENT TCP/UDP SERVER MODE\n");
        server(port, debug);
    }

    return 0;
}
