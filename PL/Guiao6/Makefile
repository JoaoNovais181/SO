
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
EXC := $(SRC:%.c=%)

CFLAGS := -g -O2 -Wall -std=gnu11

all : $(EXC)

clean:
	rm -rf *.o $(EXC)