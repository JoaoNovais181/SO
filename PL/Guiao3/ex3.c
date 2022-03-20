#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main (int argc, char *args[])
{
    if (argc<2)
    {
        printf("Deve introduzir pelo menos 1 argumento executavel\n");
        return -1;
    }

    pid_t pid;

    for (int i=1 ; i<argc ; i++)
    {
        if ((pid=fork())==0)
        {
            if (execlp(args[i], args[i], NULL))
            {
                perror("Algo correu mal");
            }
        }
    }

    for (int i=1 ; i<argc ; i++)
    {
        wait(NULL);
    }
    

    return 0;
}