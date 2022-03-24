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
            printf("Sou o filho %d e vou executar o comando %s\n", getpid(), args[i]);

            int exec_ret = execlp(args[i], args[i], NULL);
            
            perror("reched return");
            _exit(exec_ret);
        }
    }

    for (int i=1 ; i<argc ; i++)
    {
        printf("Sou o pai e estou a espera dos filhos\n");
        wait(NULL);
    }
    printf("Todos os filhos executaram\n");
    

    return 0;
}