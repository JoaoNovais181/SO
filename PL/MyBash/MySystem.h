#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#ifndef MYSYSTEM
#define MYSYSTEM


int mysystem(char **exec_args, int argc);
// int mysystem(char ***exec_args, int *argc, char **tokens);



#endif