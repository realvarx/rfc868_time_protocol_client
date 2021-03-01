CC = gcc
CFLAGS = -Wall
OBJ = atdate

all: atdate
#	$(CC) main.c server.c udp_client.c tcp_client.c -o $(OBJ)

#CFLAGS=-include /usr/include/errno.h

var1 = |          _      _       _         |
var2 = |     /\  | |    | |     | |        |
var3 = |    /  \ | |_ __| | __ _| |_ ___   |
var4 = |   / /\ \| __/ _` |/ _` | __/ _ \  |
var5 = |  / ____ \ || (_| | (_| | ||  __/  |
var6 = | /_/    \_\__\__,_|\__,_|\__\___|  |
var7 = |                                   |
var8 = |Álvaro Merino Argumánez - 100406665|

$(info $(var1))
$(info $(var2))
$(info $(var3))
$(info $(var4))
$(info $(var5))
$(info $(var6))
$(info $(var7))
$(info $(var8))

# http://www.it.uc3m.es/~pedmume/asignaturas/2005/LAO/Lab2/index_es.html

atdate: main.o server.o udp_client.o tcp_client.o
	$(CC) main.o server.o udp_client.o tcp_client.o -o $(OBJ)

main.o: main.c udp_client.h tcp_client.h
	gcc -c main.c

server.o : server.c
	gcc -c server.c

udp_client.o: udp_client.c udp_client.h
	gcc -c udp_client.c

tcp_client.o: tcp_client.c tcp_client.h
	gcc -c tcp_client.c

clean:
	rm -f *.o atdate main.o
