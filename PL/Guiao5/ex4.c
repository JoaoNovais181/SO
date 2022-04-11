#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_MAX 1024

/*  
    Escreva um programa que emule o funcionamento do interpretador de comandos na execução encadeada
    de ls /etc | wc -l.
*/



int main ()
{
    int filedes[2];

    if (pipe(filedes))
    {
        strerror(errno);
        exit(errno);
    }

    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        strerror(errno);
        exit(errno);
        break;
    case 0:

        close(filedes[1]);


        if (dup2(filedes[0], 0)==-1)
        {
            strerror(errno);
            exit(errno);
        }
        execlp("wc", "wc", "-l", NULL);
        close(filedes[0]);
        _exit(-1);
        break;
    default:

        close(filedes[0]);

        if (dup2(filedes[1], 1)==-1)
        {
            strerror(errno);
            exit(errno);
        }

        execlp("ls", "ls", "/etc", NULL);
        close(filedes[1]);
        wait(NULL);

        break;
    }

    return 0;
}