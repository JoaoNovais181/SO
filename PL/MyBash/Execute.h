#ifndef EXECUTE
#define EXECUTE

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "MySystem.h"

int execute (const char *command);

#endif