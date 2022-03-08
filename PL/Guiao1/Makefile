SRCS = $(wildcard *.c)

PROGS = $(patsubst %.c,%,$(SRCS))

CC = gcc
CFLAGS = -Wall -g

all: mycat mycp nl

mycat: mycat.c

mycp: mycp.c

nl: nl.o readln.o

nl.o: nl.c readln.h

rl.o: readln.c readln.h

clean:
	rm -rf $(PROGS)