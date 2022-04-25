
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

CFLAGS := -g -O2 -Wall -std=gnu11

all : client server

client: client.o
	gcc -Wall -g -ggdb3 client.o -o client

client.o: client.c
	gcc -c -r -Wall -g -ggdb3 client.c -o client.o

server: server.o readln.o
	gcc -Wall -g -ggdb3 server.o readln.o -o server

server.o: server.c readln.o 
	gcc -c -r -Wall -g -ggdb3 server.c -o server.o

readln.o : readln.c
	gcc -c -r -Wall -g -ggdb3 readln.c

clean:
	rm -rf *.o client server clientToServer