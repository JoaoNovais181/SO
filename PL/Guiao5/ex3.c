#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_MAX 1024


int main ()
{
    int filedes[2];

    if (pipe(filedes))
    {
        strerror(errno);
        exit(errno);
    }

    pid_t pid = fork();
    char buffer[BUF_MAX];

    switch (pid)
    {
        case -1:
            strerror(errno);
            exit(errno);
        case 0:

            close(filedes[1]);
            if (dup2(filedes[0], 0)==-1)
            {
                strerror(errno);
                exit(errno);
            }

            execlp("wc", "wc", NULL);
            close(filedes[0]);

            _exit(-1);
            break;
        default:

            close(filedes[0]);
            int bWritten = 0;

            while ((bWritten=read(0, buffer, BUF_MAX))>0)
            {
                write(filedes[1], buffer, bWritten);
            }

            close (filedes[1]);
            wait(NULL);
            break;
    }


    return 0;
}