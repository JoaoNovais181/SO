PROJ_NAME=mybash
 
# .c files
C_SOURCE=$(wildcard *.c)
 
# .h files
H_SOURCE=$(wildcard *.h)
 
# Object files
OBJ=$(subst .c,.o,$(C_SOURCE))
 
# Compiler and linker
CC=gcc
 
# Flags for compiler
CC_FLAGS= -Wall        \
		  -g		   \
		  -std=gnu11   \
		  -ggdb3       \
		  -no-pie      \
		  -O2		
 
# Command used at clean target
RM = rm -rf
 
#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

./%.o: ./%.c ./%.h
	$(CC) -c $< $(CC_FLAGS) -o $@

./main.o: ./main.c $(H_SOURCE)
	$(CC) -c $< $(CC_FLAGS) -o $@
 
$(PROJ_NAME): $(OBJ)
	$(CC) $^ $(CC_FLAGS) -o $@
	
objFolder:
 
clean:
	@ $(RM) ./*.o $(PROJ_NAME)

.PHONY: all clean%  