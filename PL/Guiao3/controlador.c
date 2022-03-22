#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main (int argc, char *args[])
{
    if (argc<2)
    {
        printf("Deve introduzir pelo menos 1 argumento executável!\n");
        return -1;
    }


    int *rv = calloc((argc-1),sizeof(int)), *nexecs = calloc((argc-1),sizeof(int)), running=argc-1, status;
    for (int i=0 ; i<argc-1 ; nexecs[i]=0, rv[i++]=-1);
    pid_t pid, *pids = calloc((argc-1),sizeof(pid_t));

    while (running)
    {
        for (int i=1 ; i<argc ; i++)
        {
            if (rv[i-1])
            {
                if ((pid = fork()) == 0)
                {
                    if (execlp(args[i], args[i], NULL))
                    {
                        perror("Algo correu mal");
                        exit(errno);
                    }
                }
                else
                {
                    pids[i-1] = pid; 
                    nexecs[i-1]++;
                }
            }
            else 
            {
                pids[i-1] = -1;
            }
        }
        for (int i=0 ; i<argc-1 ; i++)
        {
            if (pids[i]==-1) continue;
            waitpid(pids[i], &status, 0);
            if (WIFEXITED(status))
            {
                int st = WEXITSTATUS(status);
                if (st==0 && rv[i]!=0)
                {
                    rv[i]=st;
                    running--;
                }
            }
        }
    }

    for (int i=0 ; i<argc-1 ; i++)
        printf("%s %d\n", args[i+1], nexecs[i]);

    return 0;
}