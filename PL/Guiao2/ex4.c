#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main ()
{
    pid_t pid;
    int filho_num, status;

    for (filho_num = 1 ; filho_num<=10 ; filho_num++)
    {
        if (!(pid=fork())) // Processo filho
        {
            // printf("Filho%d : pid %d\n", filho_num, getpid());
            // printf("Filho%d : pid pai %d\n", filho_num, getppid());
            printf("[proc #%d] pid: %d\n", filho_num, getpid());
        
            _exit(filho_num);
        }
    }
    for (filho_num = 0 ; filho_num<10 ; filho_num++)
    {
        pid_t pid_filho = wait(&status);

        if (WIFEXITED(status))
        {
            // printf("Pai: recebi o valor %d para o pid %d\n", WEXITSTATUS(status), pid_filho);
            printf("[pai] process %d exited. exit code: %d\n", pid_filho, WEXITSTATUS(status));
        }
        else
        {
            printf("[pai] process %d exited\n", pid_filho);
        }
    }

    return 0;
}