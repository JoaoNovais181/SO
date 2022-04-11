#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#ifndef MySystem
#define MySystem


int mysystem(const char *command);
char **tokenize (char *command, int *N);



#endif